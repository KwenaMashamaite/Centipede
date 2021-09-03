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
#include <IME/core/engine/Engine.h>

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
    }

    ///////////////////////////////////////////////////////////////
    void GameplayScene::onEnter() {
        ime::Vector2u windowSize = engine().getWindow().getSize();
        m_grid->create(windowSize.y / TILE_SIZE, windowSize.x / TILE_SIZE);
    }
}
