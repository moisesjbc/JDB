/***
    Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)

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

#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "../utilities/game_state.hpp"
#include "../dependencies/m2g/src/drawables/sprite.hpp"

namespace jdb {

class MainMenu : public GameState
{
    public:
        /***
         * 1. Creation
         ****/
        MainMenu( Window& window );


        /***
         * 2. Destruction
         ***/
        virtual ~MainMenu() = default;

    protected:
        /***
         * 3. GameState interface
         ***/
        virtual void init();
        virtual void handleEvents();
        virtual void update();
        virtual void draw();
        virtual void pause();
        virtual void resume();


    private:
        m2g::SpritePtr menuText_;
};

} // namespace jdb

#endif // MAIN_MENU_HPP
