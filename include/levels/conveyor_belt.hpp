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

#ifndef CONVEYOR_BELT_HPP
#define CONVEYOR_BELT_HPP

#include <tinyxml2.h>

namespace jdb {

class ConveyorBelt
{
    private:
        float initialSpeed_;
        float speedStep_;
        unsigned int timeLapse_;

        // Current speed
        float speed_;

        unsigned int timeSinceLastUpdate_;

    public:
        /***
         * 1. Initialization
         ***/
        void load( tinyxml2::XMLElement* xmlElement, float difficultyFactor );


        /***
         * 2. Getters
         ***/
        float getInitialSpeed() const ;
        float getSpeedStep() const ;
        unsigned int getTimeLapse() const ;
        float getSpeed() const ;


        /***
         * 3. Updating
         ***/
        void reset();
        void update( unsigned int ms );
};

} // namespace jdb

#endif // CONVEYOR_BELT_HPP
