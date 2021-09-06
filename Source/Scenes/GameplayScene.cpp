////////////////////////////////////////////////////////////////////////////////
// Centipede clone
//
// Copyright (c) 2021 Kwena Mashamaite (kwena.mashamaite1@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

#include "Source/Scenes/GameplayScene.h"
#include "Source/Actors/MushroomField.h"
#include "Source/Actors/Player.h"
#include "Source/Actors/Scorpion.h"
#include <IME/core/engine/Engine.h>
#include <IME/utility/Utils.h>
#include <IME/core/physics/grid/KeyboardGridMover.h>
#include <cassert>

namespace centpd {
    ///////////////////////////////////////////////////////////////
    namespace {
        const unsigned int TILE_SIZE = 16;
    }

    ///////////////////////////////////////////////////////////////
    GameplayScene::GameplayScene() :
        m_shouldFire{false},
        m_playerAreaHeight{0}
    {}

    ///////////////////////////////////////////////////////////////
    GameplayScene::Ptr GameplayScene::create() {
        return std::make_unique<GameplayScene>();
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onInit() {
        m_playerAreaHeight = sCache().getPref("PLAYER_AREA_HEIGHT").getValue<int>();

        // Create the grid
        createTilemap(TILE_SIZE, TILE_SIZE);
        m_grid = std::make_unique<Grid>(tilemap(), gameObjects());
        ime::Vector2u windowSize = engine().getWindow().getSize();
        m_grid->create( windowSize.y / TILE_SIZE - ((m_grid->getRows() + 2) % TILE_SIZE), windowSize.x / TILE_SIZE - ((m_grid->getCols() + 3) % TILE_SIZE));

        // Destroy inactive objects
        engine().onFrameEnd([this] {
            gameObjects().removeIf([](const ime::GameObject* actor) {
                return !actor->isActive();
            });
        });
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onEnter() {
        createActors();

        // Divide the grid into two sections (Upper and lower half)
        const int row = (static_cast<int>(m_grid->getRows()) - 1) - m_playerAreaHeight;
        for (int col = 0; col < m_grid->getCols(); col++) {
            auto wall = ime::GameObject::create(*this);
            wall->setAsObstacle(true);
            wall->setCollisionGroup("invisibleWall");
            m_grid->addActor(std::move(wall), ime::Index{row, col});
        }

        // Shoot a bullet when the player presses space
        input().onKeyDown([this](ime::Keyboard::Key key) {
            if (key == ime::Keyboard::Key::Space) {
                m_shouldFire = true;

                ime::GridMover* playerMover = gridMovers().findByTag("playerMover");
                if (!playerMover->isTargetMoving())
                    fireBullet(gameObjects().findByTag<Player>("player"), playerMover->getCurrentTileIndex());
            }
        });

        // Spawn a scorpion every x minutes
        timer().setInterval(ime::minutes(1), [this] {
            spawnScorpion();
        });
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::createActors() {
        MushroomField::create(*m_grid, sCache().getPref("NUM_MUSHROOMS").getValue<unsigned int>());
        createPlayer();
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::createPlayer() {
        auto startPos = ime::Index{static_cast<int>(m_grid->getRows() - 1), static_cast<int>((m_grid->getCols() - 1) / 2)};
        auto lives = sCache().getPref("PLAYER_LIVES").getValue<int>();
        auto* player = static_cast<Player*>(m_grid->addActor(Player::create(*this, lives), startPos));

        // By default the player can fire a bullet, so we give them one
        Bullet::Ptr bullet = Bullet::create(*this);
        player->setBullet(bullet.get());
        gameObjects().add(std::move(bullet));

        // Players grid movement controller
        auto playerSpeed = sCache().getPref("PLAYER_SPEED").getValue<float>();
        auto playerMover = ime::KeyboardGridMover::create(tilemap(), player);
        playerMover->setTag("playerMover");
        playerMover->setMovementTrigger(ime::MovementTrigger::OnKeyDownHeld);
        playerMover->setMaxLinearSpeed(ime::Vector2f{playerSpeed, playerSpeed});

        // Since we are using ime::GridMover to move the bullet, it must be
        // in the grid before starting the movement. But we want to fire only
        // when the player is not moving so that the bullet appears to be coming
        // out the mouth of the player
        playerMover->onAdjacentMoveEnd([player, this](ime::Index index) {
            fireBullet(player, index);
        });

        gridMovers().addObject(std::move(playerMover));
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::spawnScorpion() {
        // The scorpion and the player do not interact directly, i.e. it must not enter the player area
        int row = ime::utility::generateRandomNum(0,(static_cast<int>(m_grid->getRows()) - 1) - m_playerAreaHeight);

        int colm;
        ime::Vector2i moveDirection;
        if (ime::utility::generateRandomNum(0, 1) == 0) { // Spawn from the left of the screen
            colm = 0;
            moveDirection = ime::Right;
        } else { // Spawn from the right of the screen
            colm = static_cast<int>(m_grid->getCols() - 1);
            moveDirection = ime::Left;
        }

        ime::GameObject* scorpion = m_grid->addActor(Scorpion::create(*this), ime::Index{row, colm});


        if (moveDirection == ime::Right) {
            // Horizontally flip the scorpion texture, by default the texture is facing left
            scorpion->getSprite().scale(-1.0f, 1.0f);
        }

        createGridMover("SCORPION", scorpion, moveDirection);
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::fireBullet(Player* player, ime::Index index) {
        if (m_shouldFire) {
            m_shouldFire = false;
            if (player->canShoot()) {
                Bullet *bullet = player->shoot();
                m_grid->addActor(bullet, index);

                // Give the player another bullet, when this one is destroyed
                bullet->onPropertyChange("active", [this](const ime::Property& property) {
                    if (!property.getValue<bool>()) {
                        auto *player = gameObjects().findByTag<Player>("player");
                        if (player) {
                            Bullet::Ptr bullet = Bullet::create(*this);
                            player->setBullet(bullet.get());
                            gameObjects().add(std::move(bullet));
                        }
                    }
                });

                // Create the bullet mover, at this point it no longer moves with the player
                createGridMover("BULLET", bullet, ime::Up);
            }
        }
    }

    ///////////////////////////////////////////////////////////////
    ime::GridMover* GameplayScene::createGridMover(const std::string& objType, ime::GameObject* target, ime::Vector2i dir) {
        assert(target && "A grid mover target cannot be a nullptr");
        ime::GridMover* gridMover = gridMovers().addObject(ime::GridMover::create(tilemap(), target));
        auto speed = sCache().getPref(objType + "_SPEED").getValue<float>(); // e.g BULLET_SPEED, SCORPION_SPEED etc
        gridMover->setMaxLinearSpeed(ime::Vector2f{speed, speed});

        // Automatically destroy the grid mover when its target gets destroyed
        target->onDestruction([gridMover, this] {
            gridMovers().removeById(gridMover->getObjectId());
        });

        if (dir != ime::Unknown) {
            // Automatically move the target to the next adjacent cell
            gridMover->onAdjacentMoveEnd([gridMover, dir](ime::Index) {
                gridMover->requestDirectionChange(dir);
            });

            // Start the movement
            gridMover->requestDirectionChange(dir);
        }

        // Actors other than the player are destroyed when attempting to move
        // beyond the bounds of the grid. This means they have moved outside
        // the player area
        if (objType != "PLAYER") {
            gridMover->onGridBorderCollision([gridMover] {
                gridMover->getTarget()->setActive(false);
            });
        }

        return gridMover;
    }
}
