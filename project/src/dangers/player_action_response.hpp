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

#ifndef PLAYER_ACTION_RESPONSE_HPP
#define PLAYER_ACTION_RESPONSE_HPP

#include "player_action.hpp"
#include "../dependencies/m2g/src/dependencies/tinyxml2/tinyxml2.h"
#include <string>
#include <stdexcept>

namespace jdb {

const int HP_ALL = 156474;

struct PlayerActionResponse
{
    // Conditions
    PlayerAction playerAction;
    int minHp;
    int maxHp;

    // Responses
    int hpVariation;
    int newState;
    int newDanger;

    // When this is true, the HP variation is divided by the inverse of the
    // FPS.
    bool dangerPerSecond;


    /***
     * 1. Initialization
     ***/
    PlayerActionResponse( tinyxml2::XMLElement* xmlElement );


    /***
     * 2. Auxiliar methods
     ***/
    static PlayerAction getPlayerActionFromString( std::string str );
};

} // namespace jdb

#endif // PLAYER_ACTION_RESPONSE_HPP
