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

#include <game_states/game_state.hpp>

namespace jdb {

const int EXIT_GAME = -999;

/***
 * 1. Construction
 ***/

GameState::GameState( sf::RenderWindow& window ) :
    window_( window ),
    exitState_( false )
{}


/***
 * 3. Main loop
 ***/

int GameState::run()
{
    init();

    clock_.restart();
    while( !exitState_ ){
        handleEvents();

        if( !exitState_ ){
            window_.clear();
            window_.draw( *this );
            window_.display();

            update( clock_.restart().asMilliseconds() );
        }
    }

    cleanUp();

    return exitStatus_;
}


/***
 * 5. State switching
 ***/

int GameState::switchState( GameState &newState )
{
    pause();

    const int exitStatus = newState.run();

    if( exitStatus != EXIT_GAME ){
        resume();
        clock_.restart();
    }else{
        requestGameExit();
    }

    return exitStatus;
}


void GameState::requestStateExit( int status )
{
    exitState_ = true;
    exitStatus_ = status;
}


void GameState::requestGameExit()
{
    requestStateExit(EXIT_GAME);
}


/***
 * 8. Clock management
 ***/

void GameState::resetClock()
{
    clock_.restart();
}

} // namespace jdb
