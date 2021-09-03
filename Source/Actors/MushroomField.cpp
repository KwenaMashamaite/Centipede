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

#include "Source/Actors/MushroomField.h"
#include "Source/Actors/Mushroom.h"
#include <IME/utility/Utils.h>
#include <cassert>

namespace centpd {
    ///////////////////////////////////////////////////////////////
    void MushroomField::create(Grid& grid, unsigned int numMushrooms) {
        // Prevent an infinite loop - A cell can only be occupied by one mushroom (And the player must not be blocked in its starting row (bottom row))
        assert(numMushrooms <= grid.getRows() * grid.getCols() - grid.getCols() && "The number of mushrooms must be one row less than the number of cells");

        auto randomRowGenerator = ime::utility::createRandomNumGenerator(0, grid.getRows() - 2); // Subtract 2 to exclude the player row (bottom)
        auto randomColGenerator = ime::utility::createRandomNumGenerator(0, grid.getCols() - 1);

        while (numMushrooms > 0) {
            auto index = ime::Index{randomRowGenerator(), randomColGenerator()};
            if (grid.isCellOccupied(index))
                continue;

            grid.addActor(Mushroom::create(grid.getScene()), index);
            numMushrooms--;
        }
    }
}
