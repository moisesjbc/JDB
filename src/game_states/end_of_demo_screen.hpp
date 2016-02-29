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

#include <game_states/gui_menu.hpp>
#include <TGUI/TGUI.hpp>

namespace jdb {

class EndOfDemoScreen : public GUIMenu
{
    public:
        /***
         * Construction
         ***/
        EndOfDemoScreen( sf::RenderWindow& window, const GameState& parentGameState );


    protected:
        /***
         * GUIMenu interface
         ***/
        virtual void initGUI(tgui::Gui& gui);


    private:
        /***
         * Auxiliar initialization methods
         ***/
        std::wstring readEndOfDemoText();


        /***
         * Attributes
         ***/
        const GameState& parentGameState_;
};

} // namespace jdb
#endif // END_OF_DEMO_SCREEN_HPP
