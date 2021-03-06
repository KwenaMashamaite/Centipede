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

#ifndef CENTIPEDE_PLAYER_H
#define CENTIPEDE_PLAYER_H

#include "Source/Actors/Bullet.h"
#include <IME/core/game_object/GameObject.h>

namespace centpd {
    /**
     * @brief User controlled character
     */
    class Player : public ime::GameObject {
    public:
        using Ptr = std::unique_ptr<Player>;

        /**
         * @brief Constructor
         * @param scene The scene the player belongs to
         * @param lives The initial number of lives
         */
        Player(ime::Scene &scene, int lives);

        /**
         * @brief Create a player
         * @param scene The scene the player belongs to
         * @param lives The number of lives
         */
        static Player::Ptr create(ime::Scene& scene, int lives);

        /**
         * @brief Set the bullet fired by the player
         * @param bullet The bullet to be fired by the player
         */
        void setBullet(Bullet* bullet);

        /**
         * @brief Get the name of this class in string format
         * @return The name of this class
         */
        std::string getClassName() const override;

        /**
         * @brief Set the number of lives
         * @param lives The number of lives
         */
        void setLives(int lives);

        /**
         * @brief Get the number of lives
         * @return The number of lives
         */
        int getLives() const;

        /**
         * @brief Check if the player can shoot a bullet
         * @return True if can shoot one, otherwise false
         *
         * The player can only shoot if it has a bullet
         *
         * @see setBullet
         */
        bool canShoot() const;

        /**
         * @brief Shoot a bullet
         * @return The fired bullet or a nullptr if the player cannot shoot
         *          at the moment
         *
         * @see canShoot
         */
        Bullet* shoot();

        /**
         * @brief Destructor
         */
        ~Player() override;

    private:
        int m_numLives;    //!< Player lives count
        int m_posChangeId; //!< The id of a position change event listener
        Bullet* m_bullet;  //!< Bullet fired by player
    };
}

#endif //CENTIPEDE_PLAYER_H
