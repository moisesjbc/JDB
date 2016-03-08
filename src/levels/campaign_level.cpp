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

#include <levels/campaign_level.hpp>
#include <level_ui/campaign_level_ui.hpp>

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
    tinyxml2::XMLDocument xmlFile;
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
    levelIntro_ = std::unique_ptr<LevelIntro>( new LevelIntro( *this, window_, levelIndex(), newDangersIDs, levelNode->FirstChildElement( "level_book" ) ) );
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
    return (sandwichesManager_.nDangersRemoved_ == sandwichesManager_.dangersCounter_->initialNDangers());
}


bool CampaignLevel::defeat() const
{
    return (jacobHp() <= 0);
}


/***
 * 4. GameState interface
 ***/

void CampaignLevel::cleanUp()
{}


std::unique_ptr<LevelUI> CampaignLevel::generateLevelUI(m2g::GraphicsLibrary& guiGraphicsLibrary) const
{
    return std::unique_ptr<LevelUI>(
                new CampaignLevelUI(
                    [this](){ return jacobHp(); },
                    std::move(guiGraphicsLibrary.getTilesetByName("health.png")),
                    [this](){ return acumScore_ + levelScore_; },
                    std::move(guiGraphicsLibrary.getTilesetByName("score.png")),
                    [this](){ return toolIndex(); },
                    std::move(guiGraphicsLibrary.getTilesetByName("tool_selector.png")),
                    [this](){ return sandwichesManager_.nDangersRemoved_ / static_cast<float>(sandwichesManager_.dangersCounter_->initialNDangers()) * 100.0f; },
                    std::move(guiGraphicsLibrary.getTilesetByName("progress.png"))
                )
                );
}


bool CampaignLevel::updatePlayerProfile(Profile &playerProfile) const
{
    if(levelIndex() > playerProfile.nextCampaignLevel()){
        playerProfile.setNextCampaignLevel(levelIndex());
        return true;
    }
    return false;
}

} // namespace jdb
