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

#include "survival_level.hpp"

namespace jdb {

SurvivalLevel::SurvivalLevel( sf::RenderWindow& window, SoundManager* soundManager, unsigned int levelIndex ) :
    Level( window, soundManager, levelIndex )
{}


/***
 * 2. Level loading
 ***/

bool SurvivalLevel::load( unsigned int index )
{
    tinyxml2::XMLNode* levelNode = nullptr;
    unsigned int i = 0;

    // Open the levels configuration file.
    xmlFile.LoadFile( (DATA_DIR_PATH + "/config/levels.xml").c_str() );

    // Iterate over the survival level XML nodes until de number index.
    levelNode = ( xmlFile.FirstChildElement( "levels" )->FirstChildElement( "survival_levels" )->FirstChildElement( "survival_level" ) );
    while( levelNode && ( i < index ) ){
        levelNode = levelNode->NextSiblingElement( "survival_level" );
        i++;
    }

    // If the index XML node doesn't exist, return false
    if( levelNode == nullptr ){
        return false;
    }

    // Load the sandwiches data.
    loadSandwichData();

    // Load the dangers data.
    tinyxml2::XMLElement* dangersXmlNode =
            (tinyxml2::XMLElement*)levelNode->FirstChildElement("dangers");
    std::vector<std::string> dangersIDs;
    std::vector<std::string> newDangersIDs;
    loadDangerData(dangersXmlNode, dangersIDs, newDangersIDs);

    // Get the conveyor belt parameters.
    conveyorBelt_.load( (tinyxml2::XMLElement*)levelNode->FirstChildElement( "speed" ) );

    levelIntro_ = std::unique_ptr<LevelIntro>( new LevelIntro( *this, window_, levelIndex_, newDangersIDs, levelNode->FirstChildElement( "level_book" ), false ) );

    return true;
}


/***
 * 3. Main loop
 ***/

bool SurvivalLevel::victory() const
{
    return false;
}


bool SurvivalLevel::defeat() const
{
    return (jacobHp_ <= 0);
}


void SurvivalLevel::updateLevelTime( unsigned int ms )
{
    levelTime_ += ms;
}


void SurvivalLevel::resetLevelTime()
{
    levelTime_ = 0;
}

} // namespace jdb
