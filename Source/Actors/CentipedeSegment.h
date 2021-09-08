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

#ifndef CENTIPEDE_CENTIPEDESEGMENT_H
#define CENTIPEDE_CENTIPEDESEGMENT_H

#include <IME/core/game_object/GameObject.h>
#include <IME/core/physics/grid/GridMover.h>

namespace centpd {
    /**
     * @brief Centipede character
     */
    class CentipedeSegment : public ime::GameObject {
    public:
        using Ptr = std::unique_ptr<CentipedeSegment>;

        /**
         * @brief The type of segment
         */
        enum class Type {
            Head, //!< Head
            Body  //!< Body
        };

        /**
         * @brief Constructor
         * @param scene The scene the segment belongs to
         * @param type The type of the segment
         */
        explicit CentipedeSegment(ime::Scene &scene, Type type = Type::Head);

        /**
         * @brief Create a CentipedeSegment
         * @param scene The scene the segment belongs to
         * @return The created segment
         */
        static CentipedeSegment::Ptr create(ime::Scene& scene, Type type = Type::Head);

        /**
         * @brief Change the type of the segment
         * @param type The new segment type
         */
        void setType(Type type);

        /**
         * @brief Get the type of the segment
         * @return The type of the segment
         */
        Type getType() const;

        /**
         * @brief Set the grid mover
         * @param gridMover The segments grid mover
         */
        void setGridMover(ime::GridMover* gridMover);

        /**
         * @brief Get the segment grid mover
         * @return The segments grid mover
         */
        ime::GridMover* getGridMover();

        /**
         * @brief Set the direction of the segment
         * @param dir The new direction
         */
        void setDirection(const ime::Vector2i& dir);

        /**
         * @brief Get th direction of the segment
         * @return The direction of the segment
         */
        const ime::Vector2i& getDirection() const;

        /**
         * @brief Get the name of th class type as string
         * @return
         */
        std::string getClassName() const override;

    private:
        /**
         * @brief Create the segments movement animations
         */
        void createAnimation();

        /**
         * @brief Move up or down by one row
         */
        void changeRow();

        /**
         * @brief Check if segment should descend or ascend then update
         */
        void updateVerticalMovement();

    private:
        Type m_type;                 //!< Head or body
        CentipedeSegment* m_link;    //!< Segment attached to this segment
        ime::Vector2i m_dir;         //!< The current direction of the segment
        ime::GridMover* m_gridMover; //!< Responsible for moving the segment in teh grid
        bool m_isSwitchingRows;      //!< A flag indicating whether or not the segment is moving up or down the grid
        bool m_isDescending;         //!< A flag indicating weather or not the segment is descending or ascending
        int m_rowChangeId;           //!< Row change handler callback id
    };
}

#endif //CENTIPEDE_CENTIPEDESEGMENT_H
