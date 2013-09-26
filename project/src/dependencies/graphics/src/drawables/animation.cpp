/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of M2G.
 *
 * M2G is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * M2G is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with M2G.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "animation.hpp"

namespace m2g {


/***
 * 1. Initialization and destruction
 ***/

Animation::Animation( std::shared_ptr< AnimationData > animationData ) :
    sprite( animationData->getTileset() )
{
    setAnimationData( animationData );
}


/***
 * 2. Getters and setters
 ***/

void Animation::setAnimationData( std::shared_ptr< AnimationData > animationData )
{
    // Set static data.
    sprite.setTileset( animationData->getTileset() );
    this->animationData = animationData;

    // Reset current state.
    setState( 0 );
}


void Animation::setState( int newState )
{
    // Get the new animation state's info.
    std::array< int, 3 > state = animationData->getState( newState );

    // Update the current animation state.
    currentState = newState;

    // Update the current tile / frame so it is now the first of the
    // new state.
    sprite.setTile( state[FIRST_FRAME] );
}


int Animation::getState() const
{
    return currentState;
}


GLuint Animation::getFrame() const
{
    return sprite.getCurrentTile();
}


/***
 * 3. Transformations
 ***/

void Animation::translate( const float& tx, const float& ty )
{
    Drawable::translate( tx, ty );
    sprite.translate( tx, ty );
}


/***
 * 4. Collision test
 ***/

const std::vector<Rect>* Animation::getCollisionRects() const
{
    return sprite.getCollisionRects();
}


/***
 * 5. Updating
 ***/

void Animation::update()
{
    // Get the current animation state's info.
    std::array< int, 3 > state = animationData->getState( currentState );

    // Get the current tile / frame.
    GLint currentTile = sprite.getCurrentTile();

    // Get the next tile / frame.
    if( currentTile < state[LAST_FRAME] ){
        currentTile++;
    }else{
        currentTile = state[BACK_FRAME];
    }

    // Update the current tile / frame.
    sprite.setTile( currentTile );
}


/***
 * 6. Drawing
 ***/

void Animation::draw( const glm::mat4& projectionMatrix ) const
{
    sprite.draw( projectionMatrix );
}

} // namespace m2g
