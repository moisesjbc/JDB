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

namespace jdb {

class CampaignLevel : public Level
{
    public:
        /***
         * 1. Initialization and destruction
         ***/
        CampaignLevel( sf::RenderWindow& window, SoundManager* soundManager, unsigned int levelIndex );


        /***
         * 2. Level loading
         ***/
        virtual bool load( unsigned int levelIndex );


        /***
         * Getters
         ***/
        static unsigned int nLevels();


        /***
         * 3. Main loop
         ***/
        virtual bool defeat() const;
        virtual bool victory() const;
        virtual void updateLevelTime( unsigned int ms );


    protected:
        /***
         * 4. GameState interface
         ***/
        virtual void cleanUp();

        // Level interface
        virtual void resetLevelTime();
        virtual void drawLevelProgress() const;
        virtual void loadGUIProgressPanel(m2g::GraphicsLibrary& guiGraphicsLibrary,
                                          std::vector<m2g::TilesetPtr>& guiTilesets,
                                          std::vector<m2g::TileSpritePtr>& guiSprites) const;
};

} // namespace jdb

#endif // CAMPAIGN_LEVEL_HPP
