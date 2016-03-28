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

#include <sandwiches/sandwich.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace jdb {


/***
 * 1. Initialization and destruction
 ***/

Sandwich::Sandwich( SandwichDataPtr sandwichData, const std::vector< DangerDataPtr >* dangerData ) :
    Animation( *( sandwichData->animationData ) ),
    nDangers_( MAX_DANGERS_PER_SANDWICH )
{
    setSandwichData( sandwichData );

    for( unsigned int i = 0; i<MAX_DANGERS_PER_SANDWICH; i++ ){
        dangers_[i] = new Danger( (*dangerData)[0] );
    }
}

Sandwich::~Sandwich()
{
    for( unsigned int i = 0; i<MAX_DANGERS_PER_SANDWICH; i++ ){
        delete dangers_[i];
    }
}


/***
 * 2. Loading
 ***/

void Sandwich::setSandwichData( SandwichDataPtr sandwichData )
{
    sandwichData_ = sandwichData;
}


bool validDanger(DangerDataPtr dangerData, int freeWidth, DangersCounter* dangersCounter)
{
    return (dangerData->baseLine.width <= freeWidth) &&
            (dangersCounter == nullptr || dangersCounter->nDangers(dangerData->id) > 0);
}

void Sandwich::populate( const std::vector< DangerDataPtr >& dangersData,
                         DangersCounter* dangersCounter )
{
    // Variables for controlling population loop exit.
    const unsigned int MAX_DANGERS = 3;
    bool exit = false;
    unsigned int i = 0;

    // Remaining free width on sandwich.
    int freeWidth = sandwichData_->baseLine.width;

    // Horizontal position for next danger.
    float newDangerX = sandwichData_->baseLine.x;

    // Variable used for selecting the next random valid danger.
    unsigned int randomDangerIndex;

    //
    nDangers_ = 0;
    while( (nDangers_ < MAX_DANGERS) && !exit ){
        // Select the valid dangers.
        std::vector<DangerDataPtr> validDangersData;
        for(DangerDataPtr dangerData : dangersData){
            if(validDanger(dangerData, freeWidth, dangersCounter)){
                validDangersData.push_back(dangerData);
            }
        }

        // Check if we found one or more valid dangers for the available space.
        if( validDangersData.size() > 0 ){
            // There are valid dangers for the available space.

            // Select a random danger between the valid ones.
            randomDangerIndex = rand() % validDangersData.size();

            // Substract the chosen danger's width to the available space.
            freeWidth -= validDangersData[randomDangerIndex]->baseLine.width;

            // Add the chosen danger data to the selected ones.
            dangers_[nDangers_]->setDangerData(validDangersData[randomDangerIndex]);

            if(dangersCounter != nullptr){
                dangersCounter->decreaseDangerCounter(validDangersData[randomDangerIndex]->id);
            }

            nDangers_++;
        }else{
            // There are not valid dangers for the available space.
            exit = true;
        }
    } // while

    int space;
    int minSpace;

    for( i = 0; i<nDangers_; i++ ){
        minSpace = ( freeWidth / (nDangers_ - i + 1) ) >> 1;
        space = rand() % (freeWidth - minSpace) + minSpace;

        dangers_[i]->setPosition( getBoundaryBox().left + newDangerX + space - dangers_[i]->getDangerData()->baseLine.x,
                                  getBoundaryBox().top + sandwichData_->baseLine.y - dangers_[i]->getDangerData()->baseLine.y );

        newDangerX += dangers_[i]->getDangerData()->baseLine.width + space;

        freeWidth -= space;
    }
}


/***
 * 3. Getters
 ***/

float Sandwich::getDamage() const
{
    float totalDamage = 0.0f;

    for( unsigned int i=0; i<nDangers_; i++ ){
        totalDamage += dangers_[i]->getDamage();
    }

    return totalDamage;
}


std::vector<std::string> Sandwich::getDangersIDs() const
{
    std::vector<std::string> dangersIDs;
    for(unsigned int i=0; i<nDangers_; i++){
        dangersIDs.push_back(dangers_[i]->getDangerData()->id);
    }
    return dangersIDs;
}


unsigned int Sandwich::nDangers() const
{
    return nDangers_;
}


/***
 * 4. Transformations
 ***/

void Sandwich::translate( const float& tx, const float& ty )
{
    Animation::move( tx, ty );

    for( unsigned int i=0; i<nDangers_; i++ ){
        dangers_[i]->move( tx, ty );
    }
}


void Sandwich::setPosition( const float& x, const float& y )
{
    Animation::setPosition( x, y );

    for( unsigned int i=0; i<nDangers_; i++ ){
        dangers_[i]->setPosition( getBoundaryBox().left+30.0f+i*75.0f, getBoundaryBox().top-75.0f );
    }
}


/***
 * 5. Updating
 ***/

void Sandwich::update(unsigned int ms, int& playerHp, unsigned int& playerScore, m2g::GraphicsLibrary& dangersGraphicsLibrary, float difficultyFactor)
{
    Animation::update( ms );

    for( unsigned int i=0; i<nDangers_; i++ ){
        dangers_[i]->update( ms, playerHp, playerScore, dangersGraphicsLibrary, difficultyFactor );
    }
}


bool Sandwich::useTool( PlayerAction playerAction,
                        TileSprite* tool,
                        int& playerHp,
                        unsigned int& playerScore,
                        m2g::GraphicsLibrary& dangersGraphicsLibrary,
                        float difficultyFactor)
{
    unsigned int i=0;

    for( ; i<nDangers_; i++ ){
        if( dangers_[i]->collide( *tool ) &&
            dangers_[i]->playerAction(playerAction, playerHp, playerScore, dangersGraphicsLibrary, difficultyFactor) ){
                return true;
        }
    }
    return false;
}


StunType Sandwich::stuns( const m2g::TileSprite &tool, ToolType toolType )
{
    unsigned int i=0;
    StunType stunType;

    for( ; i<nDangers_; i++ ){
        stunType = dangers_[i]->stuns( tool, toolType );
        if( stunType != StunType::NONE ){
            return stunType;
        }
    }
    return StunType::NONE;
}


void Sandwich::reset()
{
    for( unsigned int i=0; i<nDangers_; i++ ){
        dangers_[i]->reset();
    }
}


/***
 * 6. Drawing
 ***/

void Sandwich::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    Animation::draw( target, states );

    for( unsigned int i=0; i<nDangers_; i++ ){
        target.draw( *( dangers_[i] ), states );
    }
}

} // namespace jdb
