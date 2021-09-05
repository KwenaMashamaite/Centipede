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
         * @brief Fire the players bullet
         * @param player The player whose bullet is to be fired
         * @param index Where the bullet should be fired from
         */
        void fireBullet(Player* player, ime::Index index);

    private:
        std::unique_ptr<Grid> m_grid;  //!< The gameplay grid
        ime::GridMover* m_bulletMover; //!< A bullets grid mover (There can only be one bullet at a time)
        bool m_shouldFire;             //!< A flag indicating whether or not the player should release a bullet
    };
}

#endif //CENTIPEDE_GAMEPLAYSCENE_H
