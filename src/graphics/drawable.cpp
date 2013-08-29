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


/***
 * 3. Transformations
 ***/

void Drawable::translate( const float& tx, const float& ty )
{
    // Update the Drawable's position.
    position.x += tx;
    position.y += ty;
}

} // namespace jdb
