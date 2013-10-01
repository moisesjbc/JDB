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

#include "danger.hpp"

namespace jdb {

Danger::Danger( const std::shared_ptr< DangerData >& dangerData_ ) :
    Animation( dangerData_->animationData[ rand() % dangerData_->animationData.size()] )
{
    setDangerData( dangerData_ );
}


/***
 * 2. Getters
 ***/

float Danger::getDamage() const
{
    return hp * dangerData->damageFactor;
}


/***
 * 3. Setters
 ***/
// TODO: Overload Animation setters.
void Danger::setDangerData( const std::shared_ptr< DangerData >& dangerData_ )
{
    dangerData = dangerData_;

    hp = dangerData->initialHp;

    setState( dangerData->initialState );
}

void Danger::setState( int newState )
{
    state = newState;

    setAnimationState( (dangerData->states[newState]).animationState );
}


/***
 * 4. Updating
 ***/

void Danger::playerAction( PlayerAction playerAction )
{
    // TODO: Change
    setState( 1 );
}


} // namespace jdb
