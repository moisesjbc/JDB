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

#include "player_action_response.hpp"
#include <sstream>
#include <iostream>

namespace jdb {

/***
 * 1. Initialization
 ***/


PlayerActionResponse::PlayerActionResponse( tinyxml2::XMLElement* xmlElement )
{
    std::string token;

    // First get the string "conditions", which has the form initial_state:player_action:minHp:maxHp.
    std::istringstream conditionsStr( xmlElement->Attribute( "conditions" ) );

    // Get the player action.
    std::getline( conditionsStr, token, ':' );
    playerAction = getPlayerActionFromString( token );

    // Get the min hp.
    std::getline( conditionsStr, token, ':' );
    minHp = atoi( token.c_str() );

    // Get the max hp.
    std::getline( conditionsStr, token, ':' );
    maxHp = atoi( token.c_str() );

    // Second: get the string "responses", which has the form hpVariation:newState:newDanter.
    std::istringstream responsesStr( xmlElement->Attribute( "responses" ) );

    // Get the hp variation.
    std::getline( responsesStr, token, ':' );
    hpVariation = atoi( token.c_str() );

    // Get the new state.
    std::getline( responsesStr, token, ':' );
    newState = atoi( token.c_str() );

    // Get the new danger.
    std::getline( responsesStr, token, ':' );
    newDanger = atoi( token.c_str() );
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
    }else{
        throw std::runtime_error( "Uknown player action [" + str + "]" );
    }
}

} // namespace jdb
