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

#ifndef CAMPAIGN_LEVEL_HPP
#define CAMPAIGN_LEVEL_HPP

#include "level.hpp"

namespace jdb {

class CampaignLevel : public Level
{
    private:
        unsigned int countdown_;

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
};

} // namespace jdb

#endif // CAMPAIGN_LEVEL_HPP
