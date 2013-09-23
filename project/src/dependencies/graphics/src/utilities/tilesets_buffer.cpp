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

#include "tilesets_buffer.hpp"
#include <cstring>

namespace m2g {

/***
 * 1. Initialization and destruction
 ***/

const GLuint VERTICES_PER_TILESET = 4;
const GLuint N_DIMENSIONS = 2;
const GLuint ATTRIBUTES_PER_VERTICE = 2; // position + UV coordinates.

const GLuint TILESET_SIZE = VERTICES_PER_TILESET * N_DIMENSIONS * ATTRIBUTES_PER_VERTICE * sizeof( GLfloat );

TilesetsBuffer::TilesetsBuffer( GLuint nTilesets_ ) :
    vao( 0 ),
    vbo( 0 ),
    occupiedSize( 0 ),
    totalSize( nTilesets_ * TILESET_SIZE ),
    nTilesets( nTilesets_ )
{
    // Generate the VAO associated with this buffer and bind it as the active
    // one.
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Generate the VBO asociated with this buffer, bind it as the active one
    // and allocate the requested space.
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, totalSize, NULL, GL_STATIC_DRAW );

    // Set vertex attribute pointers.

    // We are sending 2D vertices to the vertex shader.
    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*)(0) );
    glEnableVertexAttribArray( 0 );

    // We are sending 2D texture coordinates to the shader.
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*)(2*sizeof(GLfloat)) );
    glEnableVertexAttribArray( 1 );

    std::cout << "TilesetsBuffer constructor - " << gluErrorString( glGetError() ) << std::endl;
}


TilesetsBuffer::~TilesetsBuffer()
{
    // Free allocated resources
    glDeleteVertexArrays( 1, &vao );
    glDeleteBuffers( 1, &vbo );
}


/***
 * 2. Setters and getters
 ***/


/***
 * 3. Data insertion.
 ***/

void TilesetsBuffer::insertTileset( GLfloat tileWidth, GLfloat tileHeight )
{
    GLvoid* mappedPtr = nullptr;
    GLchar* oldData = nullptr;
    const GLuint SIZE_OFFSET = 64;

    GLfloat width = 1.0f;
    GLfloat height = 1.0f;

    GLfloat tilesetData[] {
        // Bottom left
        0.0f, 0.0f,     // Vertice coordinates
        0.0f, 0.0f,     // Texture coordinates

        // Bottom right
        width, 0.0f,    // Vertice coordinates
        1.0f, 0.0f,     // Texture coordinates

        // Top left
        0.0f, height,   // Vertice coordinates
        0.0f, 1.0f,     // Texture coordinates

        // Top right
        width, height,  // Vertice coordinates
        1.0f, 1.0f      // Texture coordinates
    };

    tilesetData[4] = tileWidth;
    tilesetData[12] = tileWidth;
    tilesetData[9] = tileHeight;
    tilesetData[13] = tileHeight;

    // Bind the Buffer's VBO as the active one.
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    // Check if the adition of the new data exceeds the buffer total size.
    if( ( occupiedSize + TILESET_SIZE ) > totalSize ){
        // The adition of the new data exceeds the buffer total size. We must
        // allocate more space.

        // Allocate space in memory client for holding 	temporarily the old data.
        oldData = new char[occupiedSize];

        // Map the VBO to memory client and copy its data to previous array.
        mappedPtr = glMapBuffer( GL_ARRAY_BUFFER, GL_READ_ONLY );
        memcpy( oldData, mappedPtr, occupiedSize );
        glUnmapBuffer( GL_ARRAY_BUFFER );

        // Allocate space in VBO for both old and new data.
        glBufferData( GL_ARRAY_BUFFER, occupiedSize + TILESET_SIZE + SIZE_OFFSET, NULL, GL_STATIC_DRAW );

        // Copy the old data back to its VBO.
        glBufferSubData( GL_ARRAY_BUFFER, 0, occupiedSize, oldData );

        // Free resources.
        delete [] oldData;
    }

    // Insert the new data in the VBO.
    glBufferSubData( GL_ARRAY_BUFFER, occupiedSize, TILESET_SIZE, tilesetData );
    occupiedSize += TILESET_SIZE;

    std::cout << "Buffer::insertData - " << gluErrorString( glGetError() ) << std::endl;
}


/***
 * 4. Drawing.
 ***/

void TilesetsBuffer::draw( GLuint index )
{
    // Bind the buffer's VAO and VBO as the active ones.
    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    // Draw the requested rect.
    glDrawArrays( GL_LINE_LOOP, index*VERTICES_PER_TILESET, VERTICES_PER_TILESET );
}


/***
 * 5. Auxiliar methods
 ***/

void TilesetsBuffer::clear()
{
    occupiedSize = 0;
    nTilesets = 0;
}

} // namespace gpu_buf
