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

#ifndef TIMER_HPP
#define TIMER_HPP

#include <thread>
#include <mutex>
#include <iostream>

namespace jdb {

class Timer
{
    private:
        std::mutex mutex;

        int timeLapse;
        int seconds;

        int remainingSecondsForNewTimeLapse;

        std::function<void (void)> callbackFunction;

        bool stop_;

        std::thread* thread;

        int step_;

    public:
        /***
         * 1. Initialization
         ***/
        Timer();
        void init( unsigned int timeLapse_, std::function<void (void)> callbackFunction_, int countdown = 0 );


        /***
         * 2. Handlers
         ***/
        void stop();


        /***
         * 3. Getters
         ***/
        int getSeconds();


    private:
        /***
         * 4. Main loop
         ***/
        void loop();
};

} // namespace jdb

#endif // TIMER_HPP
