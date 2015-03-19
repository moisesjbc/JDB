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

#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include "../utilities/window.hpp"
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


        /***
         * 7. State pause / resume
         ***/
        virtual void pause() = 0;
        virtual void resume() = 0;

        sf::RenderWindow& window_;
    private:
        bool exitState_;
        int exitStatus_;
};

} // namespace jdb

#endif // GAME_STATE_HPP
