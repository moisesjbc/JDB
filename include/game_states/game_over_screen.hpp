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

#ifndef GAME_OVER_SCREEN_HPP
#define GAME_OVER_SCREEN_HPP

#include <game_states/game_state.hpp>
#include <TGUI/TGUI.hpp>

namespace jdb {

class GameOverScreen : public GameState
{
    public:
        /***
         * 1. Construction
         ***/
        GameOverScreen(sf::RenderWindow& window, const GameState& parentGameState);

    protected:
        /***
         * 2. GameState interface
         ***/
        void init();
        void handleEvents();
        void update(unsigned int ms);
        void pause();
        void resume();


    protected:
        /***
         * 3. Drawable interface
         ***/
        void draw(sf::RenderTarget &target, sf::RenderStates states) const;


        /***
         * Attributes
         ***/
        mutable tgui::Gui gui_;
        const GameState& parentGameState_;
};

} // namespace jdb

#endif // GAME_OVER_SCREEN_HPP
