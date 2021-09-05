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
#include <IME/core/engine/Engine.h>
#include <IME/core/physics/grid/KeyboardGridMover.h>

namespace centpd {
    namespace {
        const unsigned int TILE_SIZE = 16;
    }

    ///////////////////////////////////////////////////////////////
    GameplayScene::GameplayScene() :
        m_bulletMover{nullptr},
        m_shouldFire{false}
    {}

    ///////////////////////////////////////////////////////////////
    GameplayScene::Ptr GameplayScene::create() {
        return std::make_unique<GameplayScene>();
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onInit() {
        // Create the grid
        createTilemap(TILE_SIZE, TILE_SIZE);
        m_grid = std::make_unique<Grid>(tilemap(), gameObjects());
        ime::Vector2u windowSize = engine().getWindow().getSize();
        m_grid->create( windowSize.y / TILE_SIZE - ((m_grid->getRows() + 2) % TILE_SIZE), windowSize.x / TILE_SIZE - ((m_grid->getCols() + 3) % TILE_SIZE));

        // Create the bullets grid mover
        m_bulletMover = gridMovers().addObject(ime::GridMover::create(tilemap()));
        auto bulletSpeed = sCache().getPref("BULLET_SPEED").getValue<float>();
        m_bulletMover->setMaxLinearSpeed(ime::Vector2f{bulletSpeed, bulletSpeed});
        m_bulletMover->setMovementRestriction(ime::GridMover::MoveRestriction::Vertical);

        // Destroy bullet when it hits the top grid border
        m_bulletMover->onGridBorderCollision([this]{
            m_bulletMover->getTarget()->setActive(false);
        });

        // Keep the bullet moving up
        m_bulletMover->onAdjacentMoveEnd([this](ime::Index) {
            m_bulletMover->requestDirectionChange(ime::Up);
        });

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

        // Limit the player to a small section at the bottom of the grid
        const int row = (static_cast<int>(m_grid->getRows()) - 1) - sCache().getPref("PLAYER_AREA_HEIGHT").getValue<int>();
        for (int col = 0; col < m_grid->getCols(); col++) {
            auto wall = ime::GameObject::create(*this);
            wall->setAsObstacle(true);
            wall->setCollisionGroup("invisibleWall");
            m_grid->addActor(std::move(wall), ime::Index{row, col});
        }

        // Shoot bullet
        input().onKeyDown([this](ime::Keyboard::Key key) {
            if (key == ime::Keyboard::Key::Space) {
                m_shouldFire = true;

                ime::GridMover* playerMover = gridMovers().findByTag("playerMover");
                if (!playerMover->isTargetMoving())
                    fireBullet(gameObjects().findByTag<Player>("player"), playerMover->getCurrentTileIndex());
            }
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
    void GameplayScene::fireBullet(Player* player, ime::Index index) {
        if (m_shouldFire) {
            m_shouldFire = false;
            if (player->canShoot()) {
                Bullet *bullet = player->shoot();
                m_grid->addActor(bullet, index);
                m_bulletMover->setTarget(bullet);
                m_bulletMover->requestDirectionChange(ime::Up);

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
            }
        }
    }
}
