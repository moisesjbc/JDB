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

#include "danger_state.hpp"


/***
 * 1. Initialization
 ***/

namespace jdb {

DangerState::DangerState( tinyxml2::XMLElement* xmlElement )
{
    animationState = atoi( xmlElement->FirstChildElement( "animation_state" )->GetText() );

    xmlElement = xmlElement->FirstChildElement( "state_transition" );
    while( xmlElement ){
        playerActionResponses.emplace_back( xmlElement );

        xmlElement = xmlElement->NextSiblingElement( "state_transition" );
    }
}

} // nameSpace jdb.
