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

#ifndef CENTIPEDE_GAMEPLAYSCENE_H
#define CENTIPEDE_GAMEPLAYSCENE_H

#include "Source/Grid/Grid.h"
#include <IME/core/scene/Scene.h>

namespace centpd {
    class Player;

    /**
     * @brief Defines the main gameplay
     */
    class GameplayScene : public ime::Scene {
    public:
        using Ptr = std::unique_ptr<GameplayScene>; //!< Scene ptr

        /**
         * @brief Constructor
         */
        GameplayScene();

        /**
         * @brief Create a scene
         * @return A pointer to the created scene
         */
        static GameplayScene::Ptr create();

        /**
         * @brief Initialize scene
         */
        void onInit() override;

        /**
         * @brief Activate the scene
         */
        void onEnter() override;

    private:
        /**
         * @brief Create game objects
         */
        void createActors();

        /**
         * @brief Create the player actor
         */
        void createPlayer();

        /**
         * @brief Spawn a Scorpion
         */
        void spawnScorpion();

        /**
         * @brief Fire the players bullet
         * @param player The player whose bullet is to be fired
         * @param index Where the bullet should be fired from
         *
         * The bullet will only be fired if there is no active bullet (previously
         * fired) in the game
         */
        void fireBullet(Player* player, ime::Index index);

        /**
         * @brief Create a grid mover for a character
         * @param objType The type of the grid mover target in caps (e.g BULLET, PLAYER, SCORPION)
         * @param target The character to be moved by the grid mover
         * @param dir The direction in which the target moves in
         * @return A grid mover initialized for the specified character
         *
         * Note that @a dir is specified by characters that only move in
         * one direction until the end of their lifetime (e.g. a Bullet
         * always moves upwards until it is destroyed)
         */
        ime::GridMover* createGridMover(const std::string& objType, ime::GameObject* target,
            ime::Vector2i dir = ime::Unknown);

    private:
        std::unique_ptr<Grid> m_grid;      //!< The gameplay grid
        bool m_shouldFire;                 //!< A flag indicating whether or not the player should release its bullet
        int m_playerAreaHeight;            //!< The height of the player area in tiles
    };
}

#endif //CENTIPEDE_GAMEPLAYSCENE_H
