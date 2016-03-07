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

#include <dangers/player_action_response.hpp>
#include <sstream>
#include <iostream>

namespace jdb {

/***
 * 1. Initialization
 ***/

PlayerActionResponse::PlayerActionResponse(
        std::string playerAction,
        int minHp,
        int maxHp,
        DangerMutation dangerMutation ) :
    dangerMutation(dangerMutation)
{
    this->playerAction = getPlayerActionFromString(playerAction);
    this->minHp = minHp;
    this->maxHp = maxHp;
}


/***
 * 2. Auxiliar methods
 ***/

PlayerAction PlayerActionResponse::getPlayerActionFromString( std::string str )
{
    if( str == "hand_click" ){
        return PlayerAction::HAND_CLICK;
    }else if( str == "extinguisher_on"){
        return PlayerAction::EXTINGUISHER_ON;
    }else if( str == "lighter_on"){
        return PlayerAction::LIGHTER_ON;
    }else if( str == "gavel_hit" ){
        return PlayerAction::GAVEL_HIT;
    }else{
        throw std::runtime_error( "Uknown player action [" + str + "]" );
    }
}

} // namespace jdb
