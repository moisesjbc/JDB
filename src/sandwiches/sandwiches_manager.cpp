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

#include <sandwiches/sandwiches_manager.hpp>

namespace jdb {

/***
 * Resetting
 ***/

void SandwichesManager::reset()
{
    firstSandwich = 0;
    lastSandwich = sandwiches.size() - 1;
}


/***
 * Updating
 ***/

void SandwichesManager::update(int ms, int& jacobHp)
{
    // Game logic: Check if the first sandwich reached the
    // sandwiches' end point and, in that case, restart it.
    if( sandwiches[firstSandwich]->getBoundaryBox().left < SANDWICHES_END_POINT ){
        // Hurt Jacob! (muahahaha!)
        jacobHp -= sandwiches[firstSandwich]->getDamage();

        // Decrease the counter for the dangers of the sandwich.
        if(dangersCounter_ != nullptr){
            nDangersRemoved_ += sandwiches[firstSandwich]->nDangers();
        }

        if(dangersCounter_->nDangers() > 0){
            // Repopulate the sandwich.
            sandwiches[firstSandwich]->populate( dangerData_, dangersCounter_.get() );

            // Translate the sandwich behind the last one.
            sandwiches[firstSandwich]->translate(
                        sandwiches[lastSandwich]->getBoundaryBox().left
                        - sandwiches[firstSandwich]->getBoundaryBox().left
                        + DISTANCE_BETWEEN_SANDWICHES,
                        0.0f );

            // Change the indices for the first and last sandwiches.
            firstSandwich = (firstSandwich + 1) % sandwiches.size();
            lastSandwich = (lastSandwich + 1) % sandwiches.size();
        }else{
            SandwichesVector::iterator sandwichIt = sandwiches.begin();
            std::advance( sandwichIt, firstSandwich );
            sandwiches.erase( sandwichIt );
            if( sandwiches.size() ){
                firstSandwich = (firstSandwich + 1) % sandwiches.size();
            }
        }
    }
}

} // namespace jdb
