/***
    Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)

    This file is part of JDB.

    JDB is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    JDB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with JDB.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#ifndef CONVEYOR_BELT_HPP
#define CONVEYOR_BELT_HPP

#include "../dependencies/m2g/src/dependencies/tinyxml2/tinyxml2.h"

namespace jdb {

class ConveyorBelt
{
    private:
        float initialSpeed_;
        float speedStep_;
        unsigned int timeLapse_;


    public:
        /***
         * 1. Initialization
         ***/
        void load( tinyxml2::XMLElement* xmlElement );


        /***
         * 2. Getters
         ***/
        float getInitialSpeed() const ;
        float getSpeedStep() const ;
        unsigned int getTimeLapse() const ;
};

} // namespace jdb

#endif // CONVEYOR_BELT_HPP
