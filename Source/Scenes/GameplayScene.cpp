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
    const unsigned int TILE_SIZE = 16;

    ///////////////////////////////////////////////////////////////
    GameplayScene::Ptr GameplayScene::create() {
        return std::make_unique<GameplayScene>();
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onInit() {
        createTilemap(TILE_SIZE, TILE_SIZE);
        m_grid = std::make_unique<Grid>(tilemap(), gameObjects());
        ime::Vector2u windowSize = engine().getWindow().getSize();
        m_grid->create( windowSize.y / TILE_SIZE - ((m_grid->getRows() + 2) % TILE_SIZE), windowSize.x / TILE_SIZE - ((m_grid->getCols() + 3) % TILE_SIZE));
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onEnter() {
        createActors();
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::createActors() {
        // Create the player
        auto startPos = ime::Index{static_cast<int>(m_grid->getRows() - 1), static_cast<int>((m_grid->getCols() - 1) / 2)};
        auto lives = sCache().getPref("PLAYER_LIVES").getValue<int>();
        ime::GameObject* player = m_grid->addActor(Player::create(*this, lives), startPos);

        // Create mushrooms
        MushroomField::create(*m_grid, sCache().getPref("NUM_MUSHROOMS").getValue<unsigned int>());

        // Player movement controller
        auto playerSpeed = sCache().getPref("PLAYER_SPEED").getValue<float>();
        auto playerMover = ime::KeyboardGridMover::create(tilemap(), player);
        playerMover->setMovementTrigger(ime::MovementTrigger::OnKeyDownHeld);
        playerMover->setMaxLinearSpeed(ime::Vector2f{playerSpeed, playerSpeed});
        gridMovers().addObject(std::move(playerMover));

        //Limit the player to a lower section of the grid with invisible walls
        const int row = (m_grid->getRows() - 1) - sCache().getPref("PLAYER_AREA_HEIGHT").getValue<int>();
        for (int col = 0; col < m_grid->getCols(); col++) {
            auto wall = ime::GameObject::create(*this);
            wall->setAsObstacle(true);
            wall->setCollisionGroup("wall");
            m_grid->addActor(std::move(wall), ime::Index{row, col});
        }
    }
}
