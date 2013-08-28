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

#include "drawable.hpp"

namespace jdb {


/***
 * 1. Initialization
 ***/

Drawable::Drawable() :
    transformationMatrix( 1.0f ), position( 0.0f )
{
}


/***
 * 2. Getters and setters
 ***/

GLfloat Drawable::getX() const
{
    return position.x;
}


/***
 * 3. Transformations
 ***/

void Drawable::translate( const float& tx, const float& ty )
{
    transformationMatrix = glm::translate( transformationMatrix, glm::vec3( tx, ty, 0.0f ) );

    // Update the Drawable's position.
    position = transformationMatrix * glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
}

} // namespace jdb
