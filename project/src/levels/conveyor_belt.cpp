/***
    Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)

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

#include "conveyor_belt.hpp"

namespace jdb {

/***
 * 1. Initialization
 ***/

void ConveyorBelt::load( tinyxml2::XMLElement* xmlElement )
{
    initialSpeed_ = xmlElement->FloatAttribute( "initial" );
    speedStep_ = xmlElement->FloatAttribute( "step" );
    timeLapse_ = (unsigned int)( xmlElement->IntAttribute( "time_lapse" ) );
}


/***
 * 2. Getters
 ***/

float ConveyorBelt::getInitialSpeed() const
{
    return initialSpeed_;
}

float ConveyorBelt::getSpeedStep() const
{
    return speedStep_;
}


unsigned int ConveyorBelt::getTimeLapse() const
{
    return timeLapse_;
}

} // namespace jdb
