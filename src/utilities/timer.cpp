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

#include <utilities/timer.hpp>

namespace jdb {

/***
 * Construction
 ***/

Timer::Timer() :
    milliseconds_(0)
{}


void Timer::reset()
{
    milliseconds_ = 0;
}


/***
 * Getters
 ***/

unsigned int Timer::seconds() const
{
    return milliseconds_ / 1000;
}


/***
 * Updating
 ***/

void Timer::update(unsigned int ms)
{
    milliseconds_ += ms;
}

} // namespace jdb
