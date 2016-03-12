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
    return (sandwichesManager_->nDangersRemoved() == sandwichesManager_->nInitialDangers());
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
                    [this](){ return score(); },
                    std::move(guiGraphicsLibrary.getTilesetByName("score.png")),
                    [this](){ return toolIndex(); },
                    std::move(guiGraphicsLibrary.getTilesetByName("tool_selector.png")),
                    [this](){ return sandwichesManager_->nDangersRemoved() / static_cast<float>(sandwichesManager_->nInitialDangers()) * 100.0f; },
                    std::move(guiGraphicsLibrary.getTilesetByName("progress.png"))
                )
                );
}


tinyxml2::XMLElement* CampaignLevel::getLevelXmlNode(tinyxml2::XMLDocument& xmlFile, unsigned int index) const
{
    unsigned int i =0;
    tinyxml2::XMLElement* levelNode = nullptr;
    // Open the levels configuration file.
    LOG(INFO) << "Loading level configuration ...";

    // Iterate over the survival level XML nodes until de number index.
    levelNode = ( xmlFile.FirstChildElement( "levels" )->FirstChildElement( "campaign_levels" )->FirstChildElement( "campaign_level" ) );
    while( levelNode && ( i < index ) ){
        levelNode = levelNode->NextSiblingElement( "campaign_level" );
        i++;
    }

    return levelNode;
}


std::unique_ptr<LevelIntro> CampaignLevel::generateLevelIntro(const std::vector<std::string> &newDangersIDs, tinyxml2::XMLElement *levelIntroXmlNode) const
{
    return std::unique_ptr<LevelIntro>( new LevelIntro(*this, window_, levelIndex(), newDangersIDs, levelIntroXmlNode ) );
}


bool CampaignLevel::updatePlayerProfile(Profile &playerProfile) const
{
    return victory() && playerProfile.updateCampaignLevelRecordScore(levelIndex(), score());
}

} // namespace jdb
