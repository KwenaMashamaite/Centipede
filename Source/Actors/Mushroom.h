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

#ifndef CENTIPEDE_MUSHROOM_H
#define CENTIPEDE_MUSHROOM_H

#include <IME/core/game_object/GameObject.h>

namespace centpd {
    /**
     * @brief Mushroom actor
     */
    class Mushroom : public ime::GameObject {
    public:
        using Ptr = std::unique_ptr<Mushroom>;

        /**
         * @brief Constructor
         * @param scene The scene the mushroom belongs to
         */
        explicit Mushroom(ime::Scene& scene);

        /**
         * @brief Create a mushroom
         * @param scene The scene the mushroom belongs to
         */
        static Mushroom::Ptr create(ime::Scene& scene);

        /**
         * @brief Get the name of this class in string format
         * @return The name of this class
         */
        std::string getClassName() const override;

        /**
         * @brief Poison or unpoison the mushroom
         * @param poison True to poison or false to unpoison
         */
        void setPoisoned(bool poison);

        /**
         * @brief Check if the mushroom is poisoned
         * @return True if poisoned, otherwise false
         */
        bool isPoisoned() const;

        /**
         * @brief Get the number of times the mushroom has been hit by a Bullet
         * @return Bullet hir count
         */
        unsigned int getHitCount() const;

    private:
        bool m_isPoisoned;                                 //!< A flag indicating whether or not the mushroom is poisoned
        unsigned int m_hitCount;                           //!< A count of how many times the mushroom has been struck by a bullet
        static inline ime::SpriteSheet m_spriteSheet{};    //!< Holds the mushrooms textures
        static inline bool m_isSpriteSheetCreated = false; //!< A flag indicating whether or not the mushroom spritesheet is created
    };
}

#endif //CENTIPEDE_MUSHROOM_H
