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

#include "jdb.hpp"
#include "game_states/main_menu.hpp"
#include <paths.hpp>
#include <boost/filesystem.hpp>
#include <profiles/profile_json_parser.hpp>

namespace jdb {

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

    Profile playerProfile = loadProfile();

    LOG(INFO) << "Initializing main menu ...";
    MainMenu mainMenu( window_, &soundManager, playerProfile );
    LOG(INFO) << "Initializing main menu ...OK";

    mainMenu.run();
}


/***
 * Auxiliar methods
 ***/

Profile JDB::loadProfile()
{
    boost::filesystem::path savegamePath(SAVEGAME_PATH);
    if( boost::filesystem::exists(savegamePath) ){
        LOG(INFO) << "Loading savegame [" + SAVEGAME_PATH + "]";
        ProfileJSONParser profileParser;

        return profileParser.readFromJSON(savegamePath.string());
    }else{
        LOG(INFO) << "Not savegame found [" + SAVEGAME_PATH + "]";
        return Profile("Player");
    }
}


} // namespace jdb
