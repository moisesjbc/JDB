/***
    Copyright 2013 - 2016 Moises J. Bonilla Caraballo (moisesjbc)

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
#include <paths.hpp>

namespace jdb {

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;


/***
 * 1. Construction
 ***/

JDB::JDB()
{
    LOG(INFO) << "Creating SFML window ...";
    window_.create( sf::VideoMode( WINDOW_WIDTH, WINDOW_HEIGHT ), "JDB" );
    LOG(INFO) << "Creating SFML window ...OK";
}


/***
 * 2. Game run
 ***/

void JDB::run()
{
    LOG(INFO) << "Initializing sound manager ...";
    SoundManager soundManager( (DATA_DIR_PATH + "/audio").c_str() );
    LOG(INFO) << "Initializing sound manager ...OK";

    LOG(INFO) << "Initializing main menu ...";
    MainMenu mainMenu( window_, &soundManager );
    LOG(INFO) << "Initializing main menu ...OK";

    mainMenu.run();
}


} // namespace jdb
