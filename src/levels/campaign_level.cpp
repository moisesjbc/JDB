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

#include "campaign_level.hpp"

namespace jdb {

/***
 * 1. Initialization and destruction
 ***/

CampaignLevel::CampaignLevel( sf::RenderWindow& window, SoundManager* soundManager, unsigned int levelIndex, Profile& playerProfile ) :
    Level( window, soundManager, levelIndex, playerProfile )
{}


/***
 * 2. Level loading
 ***/

bool CampaignLevel::load( unsigned int index )
{
    tinyxml2::XMLElement* levelNode = nullptr;
    unsigned int i = 0;

    // Open the levels configuration file.
    LOG(INFO) << "Loading level configuration ...";
    xmlFile.LoadFile( (DATA_DIR_PATH + "/config/levels.xml").c_str() );

    // Iterate over the survival level XML nodes until de number index.
    levelNode = ( xmlFile.FirstChildElement( "levels" )->FirstChildElement( "campaign_levels" )->FirstChildElement( "campaign_level" ) );
    while( levelNode && ( i < index ) ){
        levelNode = levelNode->NextSiblingElement( "campaign_level" );
        i++;
    }

    // If the index XML node doesn't exist, return false.
    if( levelNode == nullptr ){
        return false;
    }
    LOG(INFO) << "Loading level configuration ...OK";

    // Load the sandwiches data.
    LOG(INFO) << "Loading sandwich data ...";
    loadSandwichData();
    LOG(INFO) << "Loading sandwich data ...OK";

    // Load the dangers data.
    LOG(INFO) << "Loading danger data ...";
    tinyxml2::XMLElement* dangersXmlNode =
            (tinyxml2::XMLElement*)levelNode->FirstChildElement("dangers");
    std::map<std::string, float> dangersRatios;
    std::vector<std::string> newDangersIDs;
    loadDangerData(dangersXmlNode, dangersRatios, newDangersIDs);
    LOG(INFO) << "Loading danger data ...OK";

    // Get the conveyor belt parameters.
    conveyorBelt_.load( (tinyxml2::XMLElement*)levelNode->FirstChildElement( "speed" ) );

    LOG(INFO) << "Creating level intro ...";
    levelIntro_ = std::unique_ptr<LevelIntro>( new LevelIntro( *this, window_, levelIndex_, newDangersIDs, levelNode->FirstChildElement( "level_book" ) ) );
    LOG(INFO) << "Creating level intro ...OK";

    return true;
}


/***
 * Getters
 ***/

unsigned int CampaignLevel::nLevels()
{
    tinyxml2::XMLDocument xmlFile;
    xmlFile.LoadFile( (DATA_DIR_PATH + "/config/levels.xml").c_str() );

    // Get the total number of levels.
    tinyxml2::XMLElement* levelNode = ( xmlFile.FirstChildElement( "levels" )->FirstChildElement( "campaign_levels" )->FirstChildElement( "campaign_level" ) );
    unsigned int nLevels = 0;
    while( levelNode ){
        levelNode = levelNode->NextSiblingElement( "campaign_level" );
        nLevels++;
    }

    return nLevels;
}


/***
 * 3. Main loop
 ***/

bool CampaignLevel::victory() const
{
    return (nDangersRemoved_ == dangersCounter_->initialNDangers());
}


bool CampaignLevel::defeat() const
{
    return (jacobHp_ <= 0);
}


void CampaignLevel::updateLevelTime( unsigned int ms )
{
    if( levelTime_ > ms ){
        levelTime_ -= ms;
    }else{
        levelTime_ = 0;
    }
}


/***
 * 4. GameState interface
 ***/

void CampaignLevel::cleanUp()
{}


void CampaignLevel::resetLevelTime()
{
}


void CampaignLevel::drawLevelProgress() const
{
    char buffer[10];
    float completedPercentage = nDangersRemoved_ / static_cast<float>(dangersCounter_->initialNDangers()) * 100.0f;
    sprintf(buffer, "%4.1f%%", completedPercentage);
    progressText_.setString(buffer);
    window_.draw(progressText_);
}


void CampaignLevel::loadGUIProgressPanel(m2g::GraphicsLibrary& guiGraphicsLibrary,
                                         std::vector<m2g::TilesetPtr>& guiTilesets,
                                         std::vector<m2g::TileSpritePtr>& guiSprites) const
{
    guiTilesets.push_back( guiGraphicsLibrary.getTilesetByName( "progress.png" ) );
    m2g::TileSpritePtr tileSprite =
            m2g::TileSpritePtr( new m2g::TileSprite( *( guiTilesets.back() ) ) );
    tileSprite->move( 367.0f, 0.0f );
    guiSprites.push_back( std::move( tileSprite ) );
}

} // namespace jdb
