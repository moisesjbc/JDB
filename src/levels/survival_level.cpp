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

#include <levels/survival_level.hpp>
#include <level_ui/survival_level_ui.hpp>

namespace jdb {

SurvivalLevel::SurvivalLevel( sf::RenderWindow& window, SoundManager* soundManager, unsigned int levelIndex, Profile& playerProfile ) :
    Level( window, soundManager, levelIndex, playerProfile )
{}


/***
 * 2. Level loading
 ***/

bool SurvivalLevel::load( unsigned int index )
{
    tinyxml2::XMLNode* levelNode = nullptr;
    unsigned int i = 0;

    // Open the levels configuration file.
    tinyxml2::XMLDocument xmlFile;
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
    std::map<std::string, float> dangersRatios;
    std::vector<std::string> newDangersIDs;
    loadDangerData(dangersXmlNode, dangersRatios, newDangersIDs);

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


void SurvivalLevel::drawLevelProgress() const
{

}


void SurvivalLevel::loadGUIProgressPanel(m2g::GraphicsLibrary& guiGraphicsLibrary,
                                         std::vector<m2g::TilesetPtr>& guiTilesets,
                                         std::vector<m2g::TileSpritePtr>& guiSprites) const
{
    guiTilesets.push_back( guiGraphicsLibrary.getTilesetByName( "time.png" ) );
    m2g::TileSpritePtr tileSprite =
            m2g::TileSpritePtr( new m2g::TileSprite( *( guiTilesets.back() ) ) );
    tileSprite->move( 367.0f, 0.0f );
    guiSprites.push_back( std::move( tileSprite ) );
}


std::unique_ptr<LevelUI> SurvivalLevel::generateLevelUI(m2g::GraphicsLibrary &guiGraphicsLibrary) const
{
    return std::unique_ptr<LevelUI>(
                new SurvivalLevelUI(
                    [this](){ return jacobHp_; },
                    std::move(guiGraphicsLibrary.getTilesetByName("health.png")),
                    [this](){ return acumScore_ + levelScore_; },
                    std::move(guiGraphicsLibrary.getTilesetByName("score.png")),
                    [this](){ return tool_->index(); },
                    std::move(guiGraphicsLibrary.getTilesetByName("tool_selector.png")),
                    [this](){ return levelTime_; },
                    std::move(guiGraphicsLibrary.getTilesetByName("time.png"))
                )
            );
}


bool SurvivalLevel::updatePlayerProfile(Profile &playerProfile) const
{
    return playerProfile.updateSurvivalRecordScore(score());
}

} // namespace jdb
