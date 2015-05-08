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

#include "jdb.hpp"
#include "game_states/main_menu.hpp"

namespace jdb {

const GLfloat WINDOW_WIDTH = 1024;
const GLfloat WINDOW_HEIGHT = 768;


/***
 * 1. Construction
 ***/

JDB::JDB()
{
    window_.create( sf::VideoMode( WINDOW_WIDTH, WINDOW_HEIGHT ), "JDB" );
}


/***
 * 2. Game run
 ***/

void JDB::run()
{
    SoundManager soundManager( "data/audio" );

    MainMenu mainMenu( window_, &soundManager );

    mainMenu.run();
}


} // namespace jdb
