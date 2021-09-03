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

#ifndef CENTIPEDE_GRID_H
#define CENTIPEDE_GRID_H

#include <IME/core/game_object/GameObject.h>
#include <IME/core/tilemap/TileMap.h>

namespace centpd {
    /**
     * @brief Playing grid
     */
    class Grid {
    public:
        /**
         * @brief Constructors
         * @param tileMap Third party grid
         * @param objects Scene objects container
         */
        Grid(ime::TileMap& tileMap, ime::GameObjectContainer& objects);

        /**
         * @brief Create the grid
         * @param rows The number of rows
         * @param cols The number of columns
         */
        void create(unsigned int rows, unsigned int cols);

        /**
         * @brief Add an actor to the grid
         * @param actor The actor to be added to the grid
         * @param index The index of the cell to add the actor to
         */
        void addActor(ime::GameObject::Ptr actor, ime::Index index);

        /**
         * @brief Get the number of rows
         * @return The number of rows
         */
        unsigned int getRows() const;

        /**
         * @brief Get the number of columns
         * @return The number of columns
         */
        unsigned int getCols() const;

    private:
        ime::TileMap& m_grid;
        ime::GameObjectContainer& m_gameObjects;
    };
}


#endif //CENTIPEDE_GRID_H
