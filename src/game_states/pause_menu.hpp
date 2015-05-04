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

#ifndef PAUSE_MENU_HPP
#define PAUSE_MENU_HPP

#include "game_state.hpp"
#include <SFML/Graphics/Text.hpp>
#include <TGUI/Gui.hpp>

namespace jdb {

class PauseMenu : public GameState
{
    public:
        /***
         * 1. Construction
         ***/
        PauseMenu( sf::RenderWindow& window, const GameState& parentGameState );


        /***
         * 3. GameState interface
         ***/
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    protected:
        virtual void init();
        virtual void handleEvents();
        virtual void update( unsigned int ms );
        virtual void pause();
        virtual void resume();


    private:
        const GameState& parentGameState_;
        mutable tgui::Gui gui_;
};

} // namespace jdb
#endif // PAUSE_MENU_HPP
