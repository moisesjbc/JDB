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

#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Drawable.hpp>

namespace jdb {

class GameState : public sf::Drawable
{
    public:
        /***
         * 1. Construction
         ***/
        GameState( sf::RenderWindow& window );


        /***
         * 2. Destruction
         ***/
        virtual ~GameState() = default;


        /***
         * 3. Main loop
         ***/
        int run();


    protected:
        /***
         * 4. Initialization and cleanup
         ***/
        virtual void init() = 0;
        virtual void cleanUp(){}


        /***
         * 5.  Main loop
         ***/
        virtual void handleEvents() = 0;
        virtual void update( unsigned int ms ) = 0;


        /***
         * 6. State switching
         ***/
        int switchState( GameState& newState );
        void requestStateExit( int status = 0 );
        void requestGameExit();


        /***
         * 7. State pause / resume
         ***/
        virtual void pause() = 0;
        virtual void resume() = 0;


        /***
         * 8. Clock management
         ***/
        void resetClock();


        sf::RenderWindow& window_;
    private:
        bool exitState_;
        int exitStatus_;

        sf::Clock clock_;
};

} // namespace jdb

#endif // GAME_STATE_HPP
