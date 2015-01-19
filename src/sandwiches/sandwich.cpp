/***
    Copyright 2013 - 2015 Moises J. Bonilla Caraballo (Neodivert)

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

#include "sandwich.hpp"

namespace jdb {


/***
 * 1. Initialization and destruction
 ***/

Sandwich::Sandwich( SDL_Renderer* renderer, SandwichDataPtr sandwichData, const std::vector< DangerDataPtr >* dangerData, const m2g::GraphicsLibrary& graphicsLibrary ) :
    Drawable( renderer ),
    Animation( renderer, sandwichData->animationData ),
    nDangers_( MAX_DANGERS_PER_SANDWICH )
{
    setSandwichData( sandwichData );

    for( unsigned int i = 0; i<MAX_DANGERS_PER_SANDWICH; i++ ){
        dangers_[i] = new Danger( renderer, (*dangerData)[0], graphicsLibrary );
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


void Sandwich::populate( const std::vector< DangerDataPtr >& dangerData )
{
    // Variables for controlling population loop exit.
    const unsigned int MAX_DANGERS = 3;
    bool exit = false;

    // TODO: Order vector of dangerData from greater base line width to smallest.
    unsigned int i = 0;

    // Remaining free width on sandwich.
    int freeWidth = sandwichData_->baseLine.width;

    // Horizontal position for next danger.
    float newDangerX = sandwichData_->baseLine.x;

    // Index of the first danger data which fits in the free space.
    unsigned int firstValidDanger = 0;

    // Variable used for selecting the next random valid danger.
    unsigned int randomDangerIndex;


    std::cout << "Populating sandwich" << std::endl
              << "---------------------------------------" << std::endl
              << "FreeWidth: " << freeWidth << std::endl;

    //
    nDangers_ = 0;
    while( (nDangers_ < MAX_DANGERS) && !exit ){

        // Search the first valid danger which fits in the available space.
        std::cout << "Serching first valid danger for available width (" << freeWidth << ")...";
        firstValidDanger = 0;
        while( ( firstValidDanger < dangerData.size() ) &&
               ( dangerData[firstValidDanger]->baseLine.width > freeWidth ) ){
            firstValidDanger++;
        }

        // Check if we found one or more valid dangers for the available space.
        if( firstValidDanger < dangerData.size() ){
            // There are valid dangers for the available space.

            std::cout << "FOUND (first: " << firstValidDanger << ")" << std::endl;

            // Select a random danger between the valid ones.
            randomDangerIndex = rand() % (dangerData.size() - firstValidDanger) + firstValidDanger;

            std::cout << "Selected danger: " << randomDangerIndex << std::endl;

            // Substract the chosen danger's width to the available space.
            freeWidth -= dangerData[randomDangerIndex]->baseLine.width;

            // Add the chosen danger data to the selected ones.
            dangers_[nDangers_]->setDangerData( dangerData[randomDangerIndex] );

            nDangers_++;
        }else{
            // There are not valid dangers for the available space.
            std::cout << "NOT FOUND" << std::endl;
            exit = true;
        }
    } // while


    std::cout << "Setting dangers (freeWidth: " << freeWidth << ")" << std::endl;

    int space;
    int minSpace;

    for( i = 0; i<nDangers_; i++ ){
        //if( freeWidth > 0 ){
            minSpace = ( freeWidth / (nDangers_ - i + 1) ) >> 1;
            space = rand() % (freeWidth - minSpace) + minSpace;
        //}else{
        //    minSpace = space = 0;
        //}

        std::cout << "minSpace: " << minSpace << std::endl
                  << "space: " << space << std::endl;

        dangers_[i]->moveTo( boundaryBox.x + newDangerX + space - dangers_[i]->getDangerData()->baseLine.x,
                             boundaryBox.y + sandwichData_->baseLine.y - dangers_[i]->getDangerData()->baseLine.y );

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


/***
 * 4. Transformations
 ***/

void Sandwich::translate( const float& tx, const float& ty )
{
    Drawable::translate( tx, ty );

    for( unsigned int i=0; i<nDangers_; i++ ){
        dangers_[i]->translate( tx, ty );
    }
}


void Sandwich::moveTo( const float& x, const float& y )
{
    Drawable::moveTo( x, y );

    for( unsigned int i=0; i<nDangers_; i++ ){
        dangers_[i]->moveTo( boundaryBox.x+30.0f+i*75.0f, boundaryBox.y-75.0f );
    }
}


/***
 * 5. Updating
 ***/

void Sandwich::update()
{
    Animation::update();

    for( unsigned int i=0; i<nDangers_; i++ ){
        dangers_[i]->update();
    }
}


bool Sandwich::useTool( PlayerAction playerAction, Sprite* tool,
                        unsigned int& score,
                        unsigned int& hpBonus )
{
    unsigned int i=0;

    for( ; i<nDangers_; i++ ){
        if( dangers_[i]->collide( *tool ) &&
            dangers_[i]->playerAction( playerAction, score, hpBonus ) ){
                return true;
        }
    }
    return false;
}


StunType Sandwich::stuns( const m2g::Sprite &tool, ToolType toolType )
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

void Sandwich::draw() const
{
    Animation::draw();

    for( unsigned int i=0; i<nDangers_; i++ ){
        dangers_[i]->draw();
    }
}


} // namespace jdb
