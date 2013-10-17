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


void Rect::loadFromXML( tinyxml2::XMLElement* xmlElement )
{
    x = xmlElement->FloatAttribute( "x" );
    y = xmlElement->FloatAttribute( "y" );
    width = xmlElement->FloatAttribute( "w" );
    height = xmlElement->FloatAttribute( "h" );
}


bool Rect::collide( const Rect& b ) const
{
    return  (
        ( x < ( b.x + b.width ) ) &&
        ( ( x + width ) > b.x ) &&
        ( y < ( b.y + b.height ) ) &&
        ( ( y + height ) > b.y )
            );
}


/***
 * 1. Initialization
 ***/

Drawable::Drawable()
{
    boundaryBox = { 0.0f, 0.0f, 0.0f, 0.0f };
}


/***
 * 2. Getters and setters
 ***/

GLfloat Drawable::getX() const
{
    return boundaryBox.x;
}

glm::vec2 Drawable::getPosition() const
{
    return glm::vec2( boundaryBox.x, boundaryBox.y );
}


/***
 * 3. Transformations
 ***/

void Drawable::translate( const float& tx, const float& ty )
{
    // Update the Sprite's position.
    boundaryBox.x += tx;
    boundaryBox.y += ty;
}


void Drawable::moveTo( const float& x, const float& y )
{
    // Update the Sprite's position.
    boundaryBox.x = x;
    boundaryBox.y = y;
}


/***
 * 4. Collision test
 ***/

bool Drawable::collide( const Drawable& b ) const
{
    const std::vector<Rect>* aRects = nullptr;
    const std::vector<Rect>* bRects = nullptr;

    const Rect* aBoundaryBox = getBoundaryBox();
    const Rect* bBoundaryBox = b.getBoundaryBox();

    if( !aBoundaryBox->collide( *bBoundaryBox ) ){
        return false;
    }

    aRects = getCollisionRects();
    bRects = b.getCollisionRects();

    const glm::vec2 bPosition = b.getPosition();

    Rect aRect, bRect;

    for( unsigned int i=0; i<aRects->size(); i++ ){
        aRect.x = ( (*aRects)[i] ).x + boundaryBox.x;
        aRect.y = ( (*aRects)[i] ).y + boundaryBox.y;
        aRect.width = ( (*aRects)[i] ).width;
        aRect.height = ( (*aRects)[i] ).height;

        for( unsigned int j=0; j<bRects->size(); j++ ){
            bRect.x = ( (*bRects)[j] ).x + bPosition.x;
            bRect.y = ( (*bRects)[j] ).y + bPosition.y;
            bRect.width = ( (*bRects)[j] ).width;
            bRect.height = ( (*bRects)[j] ).height;

            if( aRect.collide( bRect ) ){
                return true;
            }
        }
    }

    return false;
}


const Rect* Drawable::getBoundaryBox() const
{
    return &boundaryBox;
}


/***
 * 6. Auxiliar functions
 ***/

void checkOpenGL( const char* str )
{
    char errorMsg[256];

    GLenum errorCode = glGetError();

    if( errorCode ){
        sprintf( errorMsg, "OpenGL error code at [%s] - error code: %i (%s)", str, errorCode, gluErrorString( errorCode ) );
        throw std::runtime_error( errorMsg );
    }
}

} // namespace m2g
