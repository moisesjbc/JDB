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

#include "sandwich.hpp"

namespace jdb {


/***
 * 1. Initialization and destruction
 ***/

Sandwich::Sandwich( SandwichDataPtr sandwichData, const std::vector< DangerDataPtr >* dangerData ) :
    Animation( sandwichData->animationData ),
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


bool Sandwich::useTool( PlayerAction playerAction, Sprite* tool )
{
    unsigned int i=0;

    for( ; i<nDangers_; i++ ){
        if( dangers_[i]->collide( *tool ) &&
            dangers_[i]->playerAction( playerAction ) ){
                return true;
        }
    }
    return false;
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

void Sandwich::draw( const glm::mat4& projectionMatrix ) const
{
    Animation::draw( projectionMatrix );

    for( unsigned int i=0; i<nDangers_; i++ ){
        dangers_[i]->draw( projectionMatrix );
    }
}


} // namespace jdb
