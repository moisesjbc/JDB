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

#include "drawable.hpp"

namespace m2g {


/***
 * 1. Initialization
 ***/

Drawable::Drawable() :
    position( 0.0f )
{
}


/***
 * 2. Getters and setters
 ***/

GLfloat Drawable::getX() const
{
    return position.x;
}

glm::vec2 Drawable::getPosition() const
{
    return position;
}


/***
 * 3. Transformations
 ***/

void Drawable::translate( const float& tx, const float& ty )
{
    // Update the Sprite's position.
    position.x += tx;
    position.y += ty;
}


void Drawable::moveTo( const float& x, const float& y )
{
    // Update the Sprite's position.
    position.x = x;
    position.y = y;
}


/***
 * 4. Collision test
 ***/

bool Drawable::collide( const Drawable& b ) const
{
    const std::vector<Rect>* aRects = getCollisionRects();
    const std::vector<Rect>* bRects = b.getCollisionRects();

    const glm::vec2 bPosition = b.getPosition();

    Rect aRect, bRect;

    for( unsigned int i=0; i<aRects->size(); i++ ){
        aRect.x = ( (*aRects)[i] ).x + position.x;
        aRect.y = ( (*aRects)[i] ).y + position.y;
        aRect.width = ( (*aRects)[i] ).width;
        aRect.height = ( (*aRects)[i] ).height;

        for( unsigned int j=0; j<bRects->size(); j++ ){
            bRect.x = ( (*bRects)[j] ).x + bPosition.x;
            bRect.y = ( (*bRects)[j] ).y + bPosition.y;
            bRect.width = ( (*bRects)[j] ).width;
            bRect.height = ( (*bRects)[j] ).height;

            if(
                ( aRect.x < ( bRect.x + bRect.width ) ) && // 4
                ( ( aRect.x + aRect.width ) > bRect.x ) && // 3
                ( aRect.y < ( bRect.y + bRect.height ) ) && // 2
                ( ( aRect.y + aRect.height ) > bRect.y )  // 1
              ){
                return true;
            }
        }
    }

    return false;
}

} // namespace m2g
