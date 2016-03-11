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

#ifndef STATE_TIME_TRANSITION_HPP
#define STATE_TIME_TRANSITION_HPP

#include <dangers/danger_mutation.hpp>

namespace jdb {

struct StateTimeTransition {
    unsigned int minTimeout;
    unsigned int maxTimeout;

    DangerMutation dangerMutation;

    StateTimeTransition(unsigned int minTimeout, unsigned int maxTimeout, DangerMutation dangerMutation) :
        minTimeout(minTimeout),
        maxTimeout(maxTimeout),
        dangerMutation(dangerMutation)
    {}

    unsigned int generateTimeout(){
        return minTimeout + rand() % (maxTimeout - minTimeout + 1);
    }
};

} // namespace jdb

#endif // STATE_TIME_TRANSITION_HPP
