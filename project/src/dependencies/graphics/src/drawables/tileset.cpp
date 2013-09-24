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

#include "tileset.hpp"
#include <iostream>

namespace m2g {


/***
 * 1. Initialization and destruction.
 ***/

TilesetsBuffer* Tileset::tilesetsBuffer = nullptr;
unsigned int Tileset::refCount = 0;

Tileset::Tileset() :
    texture( 0 ),
    tileWidth( 0 ),
    tileHeight( 0 ),
    imageWidth( 0 ),
    imageHeight( 0 ),
    nRows( 0 ),
    nColumns( 0 ),
    nTiles( 0 ),
    bufferIndex( 0 )
{
    if( tilesetsBuffer == nullptr ){
        // If the tilesets buffer is not initialized, create it!.
        tilesetsBuffer = new TilesetsBuffer( 10 );
    }

    // Increment the references count.
    refCount++;
}

Tileset::~Tileset()
{
    // Free the OpenGL texture object.
    glDeleteTextures( 1, &texture );

    // Decrement the references count.
    refCount--;

    // If the references count reaches 0, delete the tilesets buffer.
    if( !refCount ){
        delete tilesetsBuffer;
        tilesetsBuffer = nullptr;
    }
}

} // Namespace m2g
