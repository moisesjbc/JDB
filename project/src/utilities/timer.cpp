/***
    Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)

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
    thread( nullptr )
{
}

void Timer::init()
{
    seconds = -1;
    timeLapse = 0;
    callbackFunction = nullptr;

    thread = new std::thread( &Timer::loop, this );
}


void Timer::init( unsigned int timeLapse_, void (*callbackFunction_)(int) )
{
    seconds = -1;
    timeLapse = timeLapse_;
    callbackFunction = callbackFunction_;

    thread = new std::thread( &Timer::loop, this );
}


void Timer::loop()
{
    // TODO: ¿Create loopWithCallback method.?
    //  - Keep in mind: what to do if the callback function delays the timer?
    mutex.lock();
    stop_ = false;
    //std::thread* callbackThread = nullptr;

    while( !stop_ ){
        seconds++;
        mutex.unlock();

        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

        /*callbackThread = new std::thread( std::bind( callbackFunction, getSeconds() ) );
        callbackThread->join();
        delete callbackThread;*/

        mutex.lock();
    }
    mutex.unlock();
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

} // namespace jdb
