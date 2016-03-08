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

const unsigned int N_SANDWICHES = 4;

namespace jdb {

/***
 * Construction
 ***/

SandwichesManager::SandwichesManager(std::vector<SandwichDataPtr> sandwichData,
                                     std::unique_ptr<std::vector<DangerDataPtr>> dangerData,
                                     std::unique_ptr<DangersCounter> dangersCounter,
                                     std::unique_ptr<m2g::GraphicsLibrary> dangersGraphicsLibrary) :
    sandwichData_(sandwichData),
    dangerData_(std::move(dangerData)),
    dangerGraphicsLibrary_(std::move(dangersGraphicsLibrary)),
    dangersCounter_(std::move(dangersCounter))
{
    reset();
}


/***
 * Resetting
 ***/

void SandwichesManager::reset()
{
    firstSandwich = 0;
    lastSandwich = sandwiches.size() - 1;

    dangersCounter_->reset();

    // Load the sandwiches, move them to their final positions and
    // populate them with dangers.
    sandwiches.clear();
    for( unsigned int i=0; i < N_SANDWICHES; i++ ){
        sandwiches.push_back(
                    std::unique_ptr< Sandwich >(
                        new Sandwich( sandwichData_[0], &(*dangerData_) ) ) );

        sandwiches[i]->setPosition( 1024 + i * DISTANCE_BETWEEN_SANDWICHES, 410 );

        sandwiches[i]->populate( *dangerData_, dangersCounter_.get() );
    }
}


/***
 * Getters
 ***/

unsigned int SandwichesManager::nDangersRemoved() const
{
    return nDangersRemoved_;
}


unsigned int SandwichesManager::nInitialDangers() const
{
    return dangersCounter_->initialNDangers();
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
            sandwiches[firstSandwich]->populate( *dangerData_, dangersCounter_.get() );

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
