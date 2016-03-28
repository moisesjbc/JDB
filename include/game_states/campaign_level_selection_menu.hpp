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

#ifndef CAMPAIGN_LEVEL_SELECTION_MENU_HPP
#define CAMPAIGN_LEVEL_SELECTION_MENU_HPP

#include <game_states/gui_menu.hpp>
#include <profiles/profile.hpp>
#include <TGUI/HorizontalLayout.hpp>

namespace jdb {

class CampaignLevelSelectionMenu : public GUIMenu
{
    public:
        /***
         * Construction
         ***/
        CampaignLevelSelectionMenu(sf::RenderWindow& window,
                                   SoundManager& soundManager,
                                   unsigned int nLevels,
                                   Profile& playerProfile );


    protected:
        /***
         * GUIMenu interface
         ***/
        void initGUI(tgui::Gui &gui);


        /***
         * GameState interface
         ***/
        void resume();


    private:
        /***
         * Auxiliar initializations
         ***/
        tgui::Widget::Ptr generateGameDifficultySelector();


        /***
         * Attributes
         ***/
        SoundManager& soundManager_;
        unsigned int nLevels_;
        Profile& playerProfile_;
        std::vector<tgui::Button::Ptr> playLevelButtons_;
};

} // namespace jdb

#endif // CAMPAIGN_LEVEL_SELECTION_MENU_HPP
