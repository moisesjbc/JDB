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

#include "tileset.hpp"

namespace jdb {

GLuint Tileset::vao = 0;


/***
 * 1. Initialization
 ***/

Tileset::Tileset()
{
    /*
    const GLfloat size = 0.5f;
    GLfloat vertices[] {
        -size, -size,   // Bottom left
        size, -size,    // Bottom right
        size, size,     // Top right
        -size, size     // Top left
    };

    if( !vbo ){
        glGenBuffers( 1, &vbo );
        glBindBuffer( GL_ARRAY_BUFFER, vbo );
        glBufferData(  )
    }
    */
}

void Tileset::setTileSetData( const std::shared_ptr< TilesetData >& tilesetData )
{
    this->tilesetData = tilesetData;
}

//void glBufferData(GLenum target,  GLsizeiptr size,  const GLvoid * data,  GLenum usage);

} // Namespace jdb
