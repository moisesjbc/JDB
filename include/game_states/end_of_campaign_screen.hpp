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

#ifndef END_OF_CAMPAIGN_SCREEN_HPP
#define END_OF_CAMPAIGN_SCREEN_HPP

#include <game_states/gui_menu.hpp>
#include <TGUI/TGUI.hpp>

namespace jdb {

class EndOfCampaignScreen : public GUIMenu
{
    public:
        /***
         * Construction
         ***/
        EndOfCampaignScreen( sf::RenderWindow& window, const GameState& parentGameState );


    protected:
        /***
         * GUIMenu interface
         ***/
        virtual void initGUI(tgui::Gui& gui);


        /***
         * Drawable interface
         ***/
        void draw(sf::RenderTarget &target, sf::RenderStates states) const;


    private:
        /***
         * Auxiliar initialization methods
         ***/
        std::wstring readEndOfCampaignText();


        /***
         * Attributes
         ***/
        const GameState& parentGameState_;
        sf::Texture backgroundTexture_;
        sf::Sprite backgroundSprite_;
};

} // namespace jdb

#endif // END_OF_CAMPAIGN_SCREEN_HPP
