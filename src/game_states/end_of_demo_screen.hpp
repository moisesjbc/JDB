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

#ifndef END_OF_DEMO_SCREEN_HPP
#define END_OF_DEMO_SCREEN_HPP

#include "game_state.hpp"
#include <TGUI/TGUI.hpp>

namespace jdb {

class EndOfDemoScreen : public GameState
{
    public:
        /***
         * 1. Construction
         ***/
        EndOfDemoScreen( sf::RenderWindow& window, const GameState& parentGameState );


    protected:
        /***
         * 2. GameState interface
         ***/
        virtual void init();
        virtual void handleEvents();
        virtual void update( unsigned int ms );
        virtual void pause();
        virtual void resume();


        /***
         * 3. Drawable interface
         ***/
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;


    private:
        /***
         * 4. Auxiliar initialization methods
         ***/
        std::wstring readEndOfDemoText();


        /***
         * Attributes
         ***/
        const GameState& parentGameState_;
        mutable tgui::Gui gui_;
};

} // namespace jdb
#endif // END_OF_DEMO_SCREEN_HPP
