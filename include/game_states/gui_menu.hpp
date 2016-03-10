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

#ifndef GUI_MENU_HPP
#define GUI_MENU_HPP

#include <game_states/game_state.hpp>
#include <utilities/sound_manager.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <TGUI/TGUI.hpp>

namespace jdb {

class GUIMenu : public GameState
{
    public:
        /***
         * Construction
         ***/
        GUIMenu(sf::RenderWindow& window, bool clearScreenWhenDrawing = true);


        /***
         * Destruction
         ***/
        virtual ~GUIMenu() = default;


    protected:
        /***
         * Initialization
         ***/
        virtual void initGUI(tgui::Gui& gui) = 0;


        /***
         * GameState interface
         ***/
        void init();
        void handleEvents();
        void update(unsigned int ms);
        void pause();
        void resume();


        /***
         * Drawable interface
         ***/
        void draw(sf::RenderTarget &target, sf::RenderStates states) const;


    private:
        /***
         * Attributes
         ***/
        mutable tgui::Gui gui_;
        bool clearScreenWhenDrawing_;
};

} // namespace jdb

#endif // GUI_MENU_HPP
