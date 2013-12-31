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

#include "timer.hpp"

namespace jdb {

/***
 * 1. Initialization
 ***/

Timer::Timer() :
    thread( nullptr ),
    step_( 1 )
{
}


void Timer::init( unsigned int timeLapse_, std::function<void (void)> callbackFunction_, int countdown )
{
    if( countdown ){
        seconds = countdown + 1;
        step_ = -1;
    }else{
        seconds = -1;
        step_ = 1;
    }
    remainingSecondsForNewTimeLapse = -1;
    timeLapse = timeLapse_;
    callbackFunction = callbackFunction_;
    remainingSecondsForNewTimeLapse = 0;

    thread = new std::thread( &Timer::loop, this );
}


int Timer::getSeconds()
{
    int res;

    mutex.lock();
    res = seconds;
    mutex.unlock();

    return res;
}


void Timer::stop()
{
    mutex.lock();
    stop_ = true;
    mutex.unlock();

    thread->join();
}


/***
 * 4. Main loop
 ***/

void Timer::loop()
{
    // TODO: ¿Create loopWithCallback method.?
    //  - Keep in mind: what to do if the callback function delays the timer?
    mutex.lock();
    stop_ = false;
    std::thread* callbackThread = nullptr;

    remainingSecondsForNewTimeLapse = timeLapse;

    while( !stop_ ){
        remainingSecondsForNewTimeLapse--;
        seconds += step_;

        if( remainingSecondsForNewTimeLapse == 0 ){
            remainingSecondsForNewTimeLapse = timeLapse;
            mutex.unlock();
            callbackThread = new std::thread( callbackFunction );
            callbackThread->join();
            delete callbackThread;
        }else{
            mutex.unlock();
        }

        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

        mutex.lock();
    }
    mutex.unlock();
}


}
