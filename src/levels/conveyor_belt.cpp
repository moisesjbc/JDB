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

#include <levels/conveyor_belt.hpp>

namespace jdb {

/***
 * 1. Initialization
 ***/

void ConveyorBelt::load( tinyxml2::XMLElement* xmlElement, float difficultyFactor )
{
    timeSinceLastUpdate_ = 0;

    initialSpeed_ = xmlElement->FloatAttribute( "initial" ) * difficultyFactor;
    speedStep_ = xmlElement->FloatAttribute( "step" ) * difficultyFactor;
    timeLapse_ = xmlElement->UnsignedAttribute( "time_lapse" ) * 1000;

    speed_ = initialSpeed_;
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


float ConveyorBelt::getSpeed() const
{
    return speed_;
}


/***
 * 3. Updating
 ***/

void ConveyorBelt::reset()
{
    speed_ = initialSpeed_;
    timeSinceLastUpdate_ = 0;
}


void ConveyorBelt::update(unsigned int ms)
{
    timeSinceLastUpdate_ += ms;
    if( timeSinceLastUpdate_ > timeLapse_ ){
        timeSinceLastUpdate_ -= timeLapse_;
        speed_ += speedStep_;
    }
}

} // namespace jdb
