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

#include "game_state.hpp"


/***
 * 1. Construction
 ***/

GameState::GameState()
{
    exitState_ = false;
}


/***
 * 3. Main loop
 ***/

void GameState::run()
{
    init();

    while( !exitState_ ){
        handleEvents();

        update();

        draw();
    }

    cleanUp();
}


/***
 * 5. State switching
 ***/

void GameState::switchState( GameState &newState )
{
    pause();

    newState.run();

    resume();
}


void GameState::requestStateExit()
{
    exitState_ = true;
}
