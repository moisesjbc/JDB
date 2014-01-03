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
 * 1. Initialization and destruction
 ***/

Timer::Timer() :
    thread_( nullptr ),
    step_( 1 )
{
}


void Timer::init( unsigned int countdown, unsigned int timeLapse, std::function<void (void)> callbackFunction )
{
    callbackFunction_ = callbackFunction;

    if( countdown ){
        seconds_ = countdown + 1;
        step_ = -1;
    }else{
        seconds_ = -1;
        step_ = 1;
    }

    remainingSecondsForNewTimeLapse_ = -1;
    timeLapse_ = timeLapse;
    remainingSecondsForNewTimeLapse_ = 0;
}


Timer::~Timer()
{
    if( thread_ ){
        thread_->join();
        delete thread_;
        thread_ = nullptr;
    }
}


/***
 * 2. Getters
 ***/

int Timer::getSeconds() const
{
    int res;

    mutex_.lock();
    res = seconds_;
    mutex_.unlock();

    return res;
}


/***
 * 4. Handlers
 ***/

void Timer::play()
{
    thread_ = new std::thread( &Timer::loop, this );
}


void Timer::stop()
{
    mutex_.lock();
    stop_ = true;
    mutex_.unlock();

    thread_->join();
    delete thread_;
    thread_ = nullptr;
}


/***
 * 5. Main loop
 ***/

void Timer::loop()
{
    // TODO: ¿Create loopWithCallback method.?
    //  - Keep in mind: what to do if the callback function delays the timer?
    mutex_.lock();
    stop_ = false;
    std::thread* callbackThread = nullptr;

    remainingSecondsForNewTimeLapse_ = timeLapse_;

    while( !stop_ ){
        remainingSecondsForNewTimeLapse_--;
        seconds_ += step_;

        if( remainingSecondsForNewTimeLapse_ == 0 ){
            remainingSecondsForNewTimeLapse_ = timeLapse_;
            mutex_.unlock();
            callbackThread = new std::thread( callbackFunction_ );
            callbackThread->join();
            delete callbackThread;
        }else{
            mutex_.unlock();
        }

        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

        mutex_.lock();
    }
    mutex_.unlock();
}


}
