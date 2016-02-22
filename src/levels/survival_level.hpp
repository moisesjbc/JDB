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

#ifndef SURVIVAL_LEVEL_HPP
#define SURVIVAL_LEVEL_HPP

#include "level.hpp"

namespace jdb {

class SurvivalLevel : public Level
{
    public:
        /***
         * 1. Initialization and destruction
         ***/
        SurvivalLevel( sf::RenderWindow& window, SoundManager* soundManager, unsigned int levelIndex );


        /***
         * 2. Level loading
         ***/
        virtual bool load( unsigned int levelIndex );


        /***
         * 3. Main loop
         ***/
        virtual bool victory() const;
        virtual bool defeat() const;


    protected:
        virtual void updateLevelTime( unsigned int ms );

        // Level interface
        virtual void resetLevelTime();
};

} // namespace jdb

#endif // SURVIVAL_LEVEL_HPP
