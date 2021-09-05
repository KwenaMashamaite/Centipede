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

#ifndef CENTIPEDE_BULLET_H
#define CENTIPEDE_BULLET_H

#include <IME/core/game_object/GameObject.h>

namespace centpd {
    class Player;

    class Bullet : public ime::GameObject {
    public:
        using Ptr = std::unique_ptr<Bullet>;

        /**
         * @brief Constructor
         * @param scene The scene the bullet belongs to
         */
        explicit Bullet(ime::Scene &scene);

        /**
         * @brief Create a bullet
         * @param scene The scene the bullet belongs to
         * @return The created bullet
         */
        static Bullet::Ptr create(ime::Scene& scene);

        /**
         * @brief Get the name of this class in string format
         * @return The name of this class
         */
        std::string getClassName() const override;

        /**
         * @brief Set the owner of the bullet
         * @param owner The owner of the bullet
         *
         * The owner of a bullet cannot be changed after the bullet is fired
         */
        void setOwner(Player* owner);

        /**
         * @brief Get the game object that owns the bullet
         * @return The owner of the bullet, or a nullptr if none exists
         *
         * If the bullet owner is destroyed after the bullet is fired, this
         * function will return a nullptr
         */
        Player* getOwner();
        const Player* getOwner() const;

        /**
         * @brief Fire the bullet
         * @param shooter The shooter of the bullet
         * @return True if the bullet was fired or otherwise false
         *
         * This function returns false if it does not have an owner or
         * when it is inactive
         */
        bool fire();

        /**
         * @brief Check if the bullet is fired or not
         * @return True if fired, otherwise false
         */
        bool isFired() const;

        /**
         * @brief Destructor
         */
        ~Bullet() override;

    private:
        /**
         * @brief Sync the position of the bullet with that of its owner
         */
        void syncPosition();

    private:
        Player* m_owner;  //!< Bullet shooter
        int m_destId;     //!< The id of the owners destruction id
        int m_posChangeId;
        bool m_isFired;   //!< A flag indicating whether or not the bullet is fired
    };
}

#endif //CENTIPEDE_BULLET_H
