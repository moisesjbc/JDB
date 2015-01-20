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

#include "game_state.hpp"

namespace jdb {

/***
 * 1. Construction
 ***/

GameState::GameState( Window& window ) :
    window_( window ),
    exitState_( false )
{}


/***
 * 3. Main loop
 ***/

int GameState::run()
{
    init();

    while( !exitState_ ){
        handleEvents();

        update();

        draw();
    }

    return exitStatus_;
}


/***
 * 5. State switching
 ***/

int GameState::switchState( GameState &newState )
{
    pause();

    const int exitStatus = newState.run();

    resume();

    return exitStatus;
}


void GameState::requestStateExit( int status )
{
    exitState_ = true;
    exitStatus_ = status;
}

} // namespace jdb
