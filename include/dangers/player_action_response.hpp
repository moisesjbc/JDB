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

#ifndef PLAYER_ACTION_RESPONSE_HPP
#define PLAYER_ACTION_RESPONSE_HPP

#include "player_action.hpp"
#include <tinyxml2.h>
#include <string>
#include <stdexcept>
#include <dangers/danger_mutation.hpp>

namespace jdb {

// TODO: Separate two structs "PlayerActionConditions" and
// "PlayerActionResponses" (affects XML).
struct PlayerActionResponse
{
    // Conditions
    PlayerAction playerAction;
    int minHp;
    int maxHp;

    // Responses
    DangerMutation dangerMutation;


    /***
     * 1. Initialization
     ***/
    PlayerActionResponse( std::string playerAction,
                          int minHp,
                          int maxHp,
                          DangerMutation dangerMutation );


    /***
     * 2. Auxiliar methods
     ***/
    static PlayerAction getPlayerActionFromString( std::string str );
};

} // namespace jdb

#endif // PLAYER_ACTION_RESPONSE_HPP
