/***
    Copyright 2013 - 2015 Moises J. Bonilla Caraballo (Neodivert)

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
    stunType( StunType::NONE )
{
    tinyxml2::XMLElement* xmlElement = nullptr;

    animationState = atoi( rootXMLElement->FirstChildElement( "animation_state" )->GetText() );

    xmlElement = rootXMLElement->FirstChildElement( "state_transition" );
    while( xmlElement ){
        playerActionResponses.emplace_back( xmlElement );

        xmlElement = xmlElement->NextSiblingElement( "state_transition" );
    }

    // Get the danger's stuns.
    xmlElement = rootXMLElement->FirstChildElement( "stun" );
    if( xmlElement ){
        std::string stunTypeStr = xmlElement->Attribute( "type" );
        if( stunTypeStr == "burn" ){
            stunType = StunType::BURN;
        }else if( stunTypeStr == "freezing" ){
            stunType = StunType::FREEZING;
        }else if( stunTypeStr == "electrocution" ){
            stunType = StunType::ELECTROCUTION;
        }else if( stunTypeStr == "bite" ){
            stunType = StunType::BITE;
        }else if( stunTypeStr == "hypnotism" ){
            stunType = StunType::HYPNOTISM;
        }else{
            throw std::runtime_error( "Unrecognized stun" );
        }

        xmlElement = xmlElement->FirstChildElement( "tool" );
        while( xmlElement ){
            std::string toolStr = xmlElement->GetText();
            if( toolStr == "hand" ){
                stunnedTools.insert( ToolType::HAND );
            }else if( toolStr == "extinguisher" ){
                stunnedTools.insert( ToolType::EXTINGUISHER );
            }else if( toolStr == "lighter" ){
                stunnedTools.insert( ToolType::LIGHTER );
            }else if( toolStr == "gavel" ){
                stunnedTools.insert( ToolType::GAVEL );
            }else{
                throw std::runtime_error( "Unrecognized stunned tool" );
            }

            xmlElement = xmlElement->NextSiblingElement( "tool" );
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

    // Get the danger's distance-based transition (if any).
    xmlElement = rootXMLElement->FirstChildElement( "state_distance_transition" );
    if( xmlElement != nullptr ){
        stateDistanceTransition =
                std::unique_ptr< StateDistanceTransition >( new StateDistanceTransition );
        stateDistanceTransition->distance = xmlElement->IntAttribute( "distance" );
        stateDistanceTransition->newState = xmlElement->IntAttribute( "new_state" );
    }

    // Check if we must change the current danger when the current animation
    // state ends.
    xmlElement = rootXMLElement->FirstChildElement( "random_danger_on_animation_state_end" );
    if( xmlElement != nullptr ){
        randomDangerOnAnimationStateEnd = true;
        appearanceAnimationLabel = xmlElement->Attribute( "appearance_animation" );
    }else{
        randomDangerOnAnimationStateEnd = false;
    }
}

} // nameSpace jdb.
