/***
    Copyright 2013 - 2015 Moises J. Bonilla Caraballo (Neodivert)

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

#ifndef LEVEL_INTRO_HPP
#define LEVEL_INTRO_HPP

#include "game_state.hpp"
#include <m2g/drawables/tile_sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <TGUI/TGUI.hpp>
#include <tinyxml2.h>

namespace jdb {

class LevelIntro : public GameState
{
    public:
        /***
         * 1. Construction
         ***/
        LevelIntro( const GameState& parentGameState,
                    sf::RenderWindow& window,
                    unsigned int levelIndex,
                    tinyxml2::XMLElement* levelBookXmlElement );


    protected:
        /***
         * 2. GameState interface
         ***/
        virtual void init();
        virtual void handleEvents();
        virtual void update( unsigned int ms );
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
        virtual void pause();
        virtual void resume();


    private:
        unsigned int levelIndex_;
        const GameState& parentGameState_;
        mutable tgui::Gui gui_;
};

} // namespace jdb

#endif // LEVEL_INTRO_HPP
