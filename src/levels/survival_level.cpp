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
 * 3. Main loop
 ***/

bool SurvivalLevel::victory() const
{
    return false;
}


bool SurvivalLevel::defeat() const
{
    return (jacobHp() <= 0);
}


/***
 * GameState interface
 ***/

void SurvivalLevel::update(unsigned int ms)
{
    Level::update(ms);
    timer_.update(ms);
}


std::unique_ptr<LevelUI> SurvivalLevel::generateLevelUI(m2g::GraphicsLibrary &guiGraphicsLibrary) const
{
    return std::unique_ptr<LevelUI>(
                new SurvivalLevelUI(
                    [this](){ return jacobHp(); },
                    std::move(guiGraphicsLibrary.getTilesetByName("health.png")),
                    [this](){ return score(); },
                    std::move(guiGraphicsLibrary.getTilesetByName("score.png")),
                    [this](){ return toolIndex(); },
                    std::move(guiGraphicsLibrary.getTilesetByName("tool_selector.png")),
                    [this](){ return timer_.seconds(); },
                    std::move(guiGraphicsLibrary.getTilesetByName("time.png"))
                )
            );
}


void SurvivalLevel::reset()
{
    Level::reset();
    timer_.reset();
}


tinyxml2::XMLElement* SurvivalLevel::getLevelXmlNode(tinyxml2::XMLDocument& xmlFile, unsigned int index) const
{
    tinyxml2::XMLElement* levelNode = nullptr;
    unsigned int i = 0;

    // Open the levels configuration file.
    xmlFile.LoadFile( (DATA_DIR_PATH + "/config/levels.xml").c_str() );

    // Iterate over the survival level XML nodes until de number index.
    levelNode = ( xmlFile.FirstChildElement( "levels" )->FirstChildElement( "survival_levels" )->FirstChildElement( "survival_level" ) );
    while( levelNode && ( i < index ) ){
        levelNode = levelNode->NextSiblingElement( "survival_level" );
        i++;
    }

    return levelNode;
}


std::unique_ptr<LevelIntro> SurvivalLevel::generateLevelIntro(const std::vector<std::string> &newDangersIDs, tinyxml2::XMLElement *levelIntroXmlNode) const
{
    return std::unique_ptr<LevelIntro>( new LevelIntro( *this, window_, levelIndex(), newDangersIDs, levelIntroXmlNode, false ) );
}


bool SurvivalLevel::updatePlayerProfile(Profile &playerProfile) const
{
    return playerProfile.updateSurvivalRecordScore(score());
}

} // namespace jdb
