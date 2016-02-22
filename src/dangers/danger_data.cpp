/***
    Copyright 2013 - 2016 Moises J. Bonilla Caraballo (Neodivert)

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

#include "danger_data.hpp"

namespace jdb {


/***
 * 1. Initialization
 ***/

DangerData::DangerData( tinyxml2::XMLElement* xmlElement,
                        m2g::GraphicsLibrary& dangerGraphics,
                        const std::vector< DangerDataPtr >& dangersDataVector ) :
    dangersDataVector( dangersDataVector )
{
    tinyxml2::XMLElement* dangerStateXMLElement;
    std::string dangerName;

    // Get the danger's name.
    dangerName = xmlElement->Attribute( "name" );

    // Get the danger's animation data.
    m2g::AnimationDataList animDataList = dangerGraphics.getAnimationDataByPrefix( dangerName );
    animationData = std::vector< m2g::AnimationDataPtr >{
            std::make_move_iterator(std::begin(animDataList)),
            std::make_move_iterator(std::end(animDataList)) };
    //animationData.push_back( dangerGraphics.getAnimationData( dangerName + "_01.png" ) );

    // Get the danger's general info.
    initialState = atoi( xmlElement->FirstChildElement( "initial_state")->GetText() );
    initialHp = atoi( xmlElement->FirstChildElement( "initial_hp")->GetText() );
    damageFactor = atof( xmlElement->FirstChildElement( "damage_factor")->GetText() );

    // Get the danger's base line.
    baseLine.loadFromXML( xmlElement->FirstChildElement( "base_line") );

    // Get the damage's states.
    dangerStateXMLElement = xmlElement->FirstChildElement( "danger_states" )->FirstChildElement( "danger_state" );
    while( dangerStateXMLElement ){
        states.emplace_back( dangerStateXMLElement, dangerGraphics );

        dangerStateXMLElement = dangerStateXMLElement->NextSiblingElement();
    }
}


DangerData::DangerData(std::vector<m2g::AnimationDataPtr> animationData,
                       const std::vector< DangerDataPtr >& dangersDataVector ) :
    animationData(std::move(animationData)),
    dangersDataVector(dangersDataVector)
{}

} // namespace jdb
