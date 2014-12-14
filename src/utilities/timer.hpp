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
        mutable std::mutex mutex_;

        int timeLapse_;
        int seconds_;

        int remainingSecondsForNewTimeLapse_;

        std::function<void (void)> callbackFunction_;

        bool stop_;

        std::thread* thread_;

        int step_;


    public:
        /***
         * 1. Initialization and destruction
         ***/
        Timer();
        ~Timer();

        void init( unsigned int countdown, unsigned int timeLapse, std::function<void (void)> callbackFunction );


        /***
         * 2. Getters
         ***/
        int getSeconds() const ;


        /***
         * 3. Setters
         ***/
        //void setCountdown( unsigned int countdown );
        //void setTimeLapse( unsigned int timeLapse );


        /***
         * 4. Handlers
         ***/
        void play();
        void stop();


    private:
        /***
         * 5. Main loop
         ***/
        void loop();
};

} // namespace jdb

#endif // TIMER_HPP
