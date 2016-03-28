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
#include <SFML/Graphics/RenderTarget.hpp>

const unsigned int N_SANDWICHES = 4;

namespace jdb {

/***
 * Construction
 ***/

SandwichesManager::SandwichesManager(std::vector<SandwichDataPtr> sandwichData,
                                     std::unique_ptr<std::vector<DangerDataPtr>> dangerData,
                                     std::unique_ptr<DangersCounter> dangersCounter,
                                     std::unique_ptr<m2g::GraphicsLibrary> dangersGraphicsLibrary,
                                     const ConveyorBelt& conveyorBelt) :
    dangerGraphicsLibrary_(std::move(dangersGraphicsLibrary)),
    sandwichData_(sandwichData),
    dangerData_(std::move(dangerData)),
    dangersCounter_(std::move(dangersCounter)),
    nDangersRemoved_(0),
    conveyorBelt_(conveyorBelt)
{
    reset();
}


/***
 * Resetting
 ***/

void SandwichesManager::reset()
{
    firstSandwich = 0;
    lastSandwich = sandwiches_.size() - 1;

    nDangersRemoved_ = 0;
    dangersCounter_->reset();

    // Load the sandwiches, move them to their final positions and
    // populate them with dangers.
    sandwiches_.clear();
    for( unsigned int i=0; i < N_SANDWICHES; i++ ){
        sandwiches_.push_back(
                    std::unique_ptr< Sandwich >(
                        new Sandwich( sandwichData_[0], &(*dangerData_) ) ) );

        sandwiches_[i]->setPosition( 1024 + i * DISTANCE_BETWEEN_SANDWICHES, 410 );

        sandwiches_[i]->populate( *dangerData_, dangersCounter_.get() );
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


SandwichesVector &SandwichesManager::sandwiches()
{
    return sandwiches_;
}


/***
 * Updating
 ***/

void SandwichesManager::update(int ms, int& jacobHp, unsigned int& levelScore, float difficultyFactor)
{
    // Game logic: Check if the first sandwich reached the
    // sandwiches' end point and, in that case, restart it.
    if( sandwiches_[firstSandwich]->getBoundaryBox().left < SANDWICHES_END_POINT ){
        // Hurt Jacob! (muahahaha!)
        jacobHp -= sandwiches_[firstSandwich]->getDamage();

        // Decrease the counter for the dangers of the sandwich.
        if(dangersCounter_ != nullptr){
            nDangersRemoved_ += sandwiches_[firstSandwich]->nDangers();
        }

        if(dangersCounter_->nDangers() > 0){
            // Repopulate the sandwich.
            sandwiches_[firstSandwich]->populate( *dangerData_, dangersCounter_.get() );

            // Translate the sandwich behind the last one.
            sandwiches_[firstSandwich]->translate(
                        sandwiches_[lastSandwich]->getBoundaryBox().left
                        - sandwiches_[firstSandwich]->getBoundaryBox().left
                        + DISTANCE_BETWEEN_SANDWICHES,
                        0.0f );

            // Change the indices for the first and last sandwiches.
            firstSandwich = (firstSandwich + 1) % sandwiches_.size();
            lastSandwich = (lastSandwich + 1) % sandwiches_.size();
        }else{
            SandwichesVector::iterator sandwichIt = sandwiches_.begin();
            std::advance( sandwichIt, firstSandwich );
            sandwiches_.erase( sandwichIt );
            if( sandwiches_.size() ){
                firstSandwich = (firstSandwich + 1) % sandwiches_.size();
            }
        }
    }

    // Update the sandwiches
    unsigned int i;
    for( i=0; i < sandwiches_.size(); i++ ){
        sandwiches_[i]->update( ms, jacobHp, levelScore, *dangerGraphicsLibrary_, difficultyFactor );
    }

    // Move the sandwiches
    // Conveyor belt's speed management.
    float speed = conveyorBelt_.getSpeed();
    for( i=0; i < sandwiches_.size(); i++ ){
        sandwiches_[i]->translate( -speed, 0.0f );
    }
}


/***
 * Drawable interface
 ***/

void SandwichesManager::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for( auto& sandwich : sandwiches_ ){
        target.draw( *sandwich, states );
    }
}

} // namespace jdb
