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

#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "game_state.hpp"
#include <m2g/drawables/tile_sprite.hpp>
#include <game_states/gui_menu.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <TGUI/TGUI.hpp>
#include <profiles/profile.hpp>

namespace jdb {

class MainMenu : public GUIMenu
{
    public:
        /***
         * 1. Creation
         ****/
        MainMenu( sf::RenderWindow& window, SoundManager* soundManager, Profile& playerProfile);


        /***
         * 2. Destruction
         ***/
        virtual ~MainMenu() = default;


    protected:
        /***
         * 3. GUIMenu interface
         ***/
        virtual void initGUI(tgui::Gui& gui);


        /***
         * 4. GameState interface
         ***/
        virtual void resume();


        /***
         * 5. Auxiliar initialization methods
         ***/
        tgui::VerticalLayout::Ptr generateTitleLayout() const;
        tgui::VerticalLayout::Ptr generateMenuLayout();


    private:
        sf::Font font_;
        sf::Text menuText_;
        SoundManager& soundManager_;
        Profile& playerProfile_;
        tgui::Label::Ptr currentSurvivalRecordScoreLabel_;
};

} // namespace jdb

#endif // MAIN_MENU_HPP
