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

namespace jdb {

class GameState
{
    public:
        /***
         * 1. Construction
         ***/
        GameState( Window& window );


        /***
         * 2. Destruction
         ***/
        virtual ~GameState() = default;


        /***
         * 3. Main loop
         ***/
        void run();


    protected:
        /***
         * 4. Initialization and cleanup
         ***/
        virtual void init() = 0;


        /***
         * 5.  Main loop
         ***/
        virtual void handleEvents() = 0;
        virtual void update() = 0;
        virtual void draw() = 0;


        /***
         * 6. State switching
         ***/
        void switchState( GameState& newState );
        void requestStateExit();


        /***
         * 7. State pause / resume
         ***/
        virtual void pause() = 0;
        virtual void resume() = 0;

        Window& window_;
    private:
        bool exitState_;
};

} // namespace jdb

#endif // GAME_STATE_HPP
