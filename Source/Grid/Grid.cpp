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

#include "Source/Grid/Grid.h"

namespace centpd {
    ///////////////////////////////////////////////////////////////
    Grid::Grid(ime::TileMap& tileMap, ime::GameObjectContainer& gameObjects) :
        m_grid{tileMap},
        m_gameObjects{gameObjects}
    {
        // By default, IME sort render layers by the order in which they are created
        m_grid.renderLayers().create("Mushrooms");
    }

    ///////////////////////////////////////////////////////////////
    void Grid::create(unsigned int rows, unsigned int cols) {
        m_grid.construct(ime::Vector2u{rows, cols}, '.');

#ifndef NDEBUG
        m_grid.getRenderer().setVisible(true);
#else
        m_grid.getRenderer().setVisible(false);
#endif
    }

    ///////////////////////////////////////////////////////////////
    void Grid::addActor(ime::GameObject::Ptr object, ime::Index index) {
        assert(object && "Object must not be a nullptr");
        std::string renderLayer = object->getClassName() + "s";

        m_grid.addChild(object.get(), index);
        auto group = object->getClassName();
        m_gameObjects.add(group, std::move(object), 0, renderLayer);
    }

    ///////////////////////////////////////////////////////////////
    unsigned int Grid::getRows() const {
        return m_grid.getSizeInTiles().y;
    }

    ///////////////////////////////////////////////////////////////
    unsigned int Grid::getCols() const {
        return m_grid.getSizeInTiles().x;
    }

    ///////////////////////////////////////////////////////////////
    bool Grid::isCellOccupied(const ime::Index &index) const {
        return m_grid.isTileOccupied(index);
    }

    ///////////////////////////////////////////////////////////////
    ime::Scene &Grid::getScene() {
        return m_grid.getScene();
    }
}
