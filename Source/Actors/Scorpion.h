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

#ifndef CENTIPEDE_SCORPION_H
#define CENTIPEDE_SCORPION_H

#include <IME/core/game_object/GameObject.h>
#include <IME/core/physics/grid/GridMover.h>

namespace centpd {
    /**
     * @brief Scorpion character
     */
    class Scorpion : public ime::GameObject {
    public:
        using Ptr = std::unique_ptr<Scorpion>;

        /**
         * @brief Constructor
         * @param scene The scene the game object belongs to
         */
        explicit Scorpion(ime::Scene &scene);

        /**
         * @brief Create a Scorpion character
         * @param scene The scene the character belongs to
         * @return The created character
         */
        static Scorpion::Ptr create(ime::Scene& scene);

        /**
         * @brief Get the name of this class in string format
         * @return The name of this class
         */
        std::string getClassName() const override;

    private:
        int m_hitCount; //!< The number of times the scorpion has been hit by a bullet
    };
}

#endif //CENTIPEDE_SCORPION_H
