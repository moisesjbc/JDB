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

#ifndef CAMPAIGN_LEVEL_HPP
#define CAMPAIGN_LEVEL_HPP

#include "level.hpp"
#include <utilities/game_difficulty.hpp>

namespace jdb {

class CampaignLevel : public Level
{
    public:
        /***
         * 1. Initialization and destruction
         ***/
        CampaignLevel( sf::RenderWindow& window, SoundManager* soundManager, unsigned int levelIndex, Profile& playerProfile );


        /***
         * Getters
         ***/
        static unsigned int nLevels();


        /***
         * 3. Main loop
         ***/
        virtual bool defeat() const;
        virtual bool victory() const;


    protected:
        /***
         * 4. GameState interface
         ***/
        virtual void cleanUp();

        // Level interface
        std::unique_ptr<LevelUI> generateLevelUI(m2g::GraphicsLibrary& guiGraphicsLibrary) const;
        tinyxml2::XMLElement* getLevelXmlNode(tinyxml2::XMLDocument& xmlFile, unsigned int index) const;
        std::unique_ptr<LevelIntro> generateLevelIntro(const std::vector<std::string> &newDangersIDs, tinyxml2::XMLElement *levelIntroXmlNode) const;

    private:
        bool updatePlayerProfile(Profile &playerProfile) const;
};

} // namespace jdb

#endif // CAMPAIGN_LEVEL_HPP
