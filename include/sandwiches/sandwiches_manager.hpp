/***
    Copyright 2013 - 2016 Moises J. Bonilla Caraballo (moisesjbc)

    This file is part of sandwiches-game.

    sandwiches-game is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    sandwiches-game is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with sandwiches-game.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#ifndef SANDWICHES_MANAGER
#define SANDWICHES_MANAGER

const float SANDWICHES_END_POINT = 0.0f;
const float DISTANCE_BETWEEN_SANDWICHES = 300.0f;

#include <sandwiches/sandwich.hpp>
#include <vector>

namespace jdb {

class SandwichesManager
{
    public:
        /***
         * Resetting
         ***/
        void reset();


        /***
         * Updating
         ***/
        void update(int ms, int& jacobHp);


    public:
        std::vector<SandwichDataPtr> sandwichData_;
        std::vector<DangerDataPtr> dangerData_;

        // Variables used for sandwich reseting.
        unsigned int firstSandwich;
        unsigned int lastSandwich;

        // Sandwiches
        SandwichesVector sandwiches;

        std::unique_ptr<DangersCounter> dangersCounter_;

        unsigned int nDangersRemoved_ = 0;

        std::unique_ptr< m2g::GraphicsLibrary > dangerGraphicsLibrary_;
};

} // namespace jdb


#endif // SANDWICHES_MANAGER
