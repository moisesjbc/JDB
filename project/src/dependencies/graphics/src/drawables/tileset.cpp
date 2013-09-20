/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of Graphic Librarian.
 *
 * Graphic Librarian is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Graphic Librarian is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Graphic Librarian.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "tileset.hpp"
#include <iostream>

namespace m2g {

Tileset::Tileset() :
    texture( 0 ),
    tileWidth( 0 ),
    tileHeight( 0 ),
    imageWidth( 0 ),
    imageHeight( 0 ),
    nRows( 0 ),
    nColumns( 0 ),
    nTiles( 0 ),
    vbo( 0 )
{}

Tileset::~Tileset()
{
    glDeleteBuffers( 1, &vbo );
    glDeleteTextures( 1, &texture );
}

} // Namespace m2g
