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

DangerState::DangerState( tinyxml2::XMLElement* rootXMLElement ) :
    stateTimeTransition( nullptr ),
    tauntType( TauntType::NONE )
{
    tinyxml2::XMLElement* xmlElement = nullptr;

    animationState = atoi( rootXMLElement->FirstChildElement( "animation_state" )->GetText() );

    xmlElement = rootXMLElement->FirstChildElement( "state_transition" );
    while( xmlElement ){
        playerActionResponses.emplace_back( xmlElement );

        xmlElement = xmlElement->NextSiblingElement( "state_transition" );
    }

    // Get the danger's taunts.
    xmlElement = rootXMLElement->FirstChildElement( "taunt" );
    if( xmlElement ){
        std::string tauntTypeStr = xmlElement->Attribute( "type" );
        if( tauntTypeStr == "burn" ){
            tauntType = TauntType::BURN;
        }else if( tauntTypeStr == "freezing" ){
            tauntType = TauntType::FREEZING;
        }else if( tauntTypeStr == "electrocution" ){
            tauntType = TauntType::ELECTROCUTION;
        }else if( tauntTypeStr == "bite" ){
            tauntType = TauntType::BITE;
        }else{
            throw std::runtime_error( "Unrecognized taunt" );
        }

        xmlElement = xmlElement->FirstChildElement( "tool" );
        while( xmlElement ){
            std::string toolStr = xmlElement->GetText();
            if( toolStr == "hand" ){
                tauntedTools.insert( ToolType::HAND );
            }else if( toolStr == "extinguisher" ){
                tauntedTools.insert( ToolType::EXTINGUISHER );
            }else if( toolStr == "lighter" ){
                tauntedTools.insert( ToolType::LIGHTER );
            }else if( toolStr == "gavel" ){
                tauntedTools.insert( ToolType::GAVEL );
            }else{
                throw std::runtime_error( "Unrecognized taunted tool" );
            }

            xmlElement = xmlElement->NextSiblingElement();
        }
    }

    // Get the danger's time-based transition (if any).
    xmlElement = rootXMLElement->FirstChildElement( "state_time_transition" );
    if( xmlElement != nullptr ){
        stateTimeTransition =
                std::unique_ptr< StateTimeTransition >( new StateTimeTransition );
        stateTimeTransition->minTimeout = xmlElement->IntAttribute( "min" );
        stateTimeTransition->maxTimeout = xmlElement->IntAttribute( "max" );
        stateTimeTransition->newState = xmlElement->IntAttribute( "new_state" );
    }
}

} // nameSpace jdb.
