/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of GPU Buffers.
 *
 * GPU Buffers is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * GPU Buffers is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GPU Buffers.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "buffer.hpp"
#include <cstring>


namespace gpu_buf {

/***
 * 1. Initialization and destruction
 ***/

Buffer::Buffer( GLenum target_, GLenum usage_, GLuint totalSize_ ) :
    vao( 0 ),
    vbo( 0 ),
    occupiedSize( 0 ),
    totalSize( totalSize_ ),
    vboUsage( usage_ ),
    vboTarget( target_ ),
    defaultColor( { 1.0f, 0.0f, 0.0f, 1.0f } )
{
    // Generate the VAO and VBO associated to this buffer.
    glGenVertexArrays( 1, &vao );
    glGenBuffers( 1, &vbo );

    // Allocate space for the VBO.
    glBindBuffer( vboTarget, vbo );
    glBufferData( vboTarget, totalSize, NULL, vboUsage );

    std::cout << "Buffer constructor - " << gluErrorString( glGetError() ) << std::endl;
}


Buffer::~Buffer()
{
    glDeleteVertexArrays( 1, &vao );
    glDeleteBuffers( 1, &vbo );
}


/***
 * 2. Setters and getters
 ***/

void Buffer::setVertexAttribPointer( GLuint index,  GLint size,  GLenum type,  GLboolean normalized, GLsizei stride, const GLvoid * pointer )
{
    // Bind the buffer's VAO and VBO as the active ones.
    glBindVertexArray( vao );
    glBindBuffer( vboTarget, vbo );

    // Set the vertex attribute pointer.
    glVertexAttribPointer( index, size, type, normalized, stride, pointer );

    // Enable the previous vertex attribute pointer.
    glEnableVertexAttribArray( index );
}


void Buffer::setColorUniformLocation( GLint location )
{
    colorUniformLocation = location;
}

/***
 * 3. Data insertion.
 ***/

void Buffer::insertData( GLvoid* newData, GLuint size )
{
    GLvoid* mappedPtr = nullptr;
    GLchar* oldData = nullptr;
    const GLuint SIZE_OFFSET = 64;

    // Bind the Buffer's VBO as the active one.
    glBindBuffer( vboTarget, vbo );

    // Check if the adition of the new data exceeds the buffer total size.
    if( ( occupiedSize + size ) > totalSize ){
        // The adition of the new data exceeds the buffer total size. We must
        // allocate more space.

        // Allocate space in memory client for holding 	temporarily the old data.
        oldData = new char[occupiedSize];

        // Map the VBO to memory client and copy its data to previous array.
        mappedPtr = glMapBuffer( vboTarget, GL_READ_ONLY );
        memcpy( oldData, mappedPtr, occupiedSize );
        glUnmapBuffer( vboTarget );

        // Allocate space in VBO for both old and new data.
        glBufferData( vboTarget, occupiedSize + size + SIZE_OFFSET, NULL, vboUsage );

        // Copy the old data back to its VBO.
        glBufferSubData( vboTarget, 0, occupiedSize, oldData );

        // Free resources.
        delete [] oldData;
    }

    // Insert the new data in the VBO.
    glBufferSubData( vboTarget, occupiedSize, size, newData );
    occupiedSize += size;

    std::cout << "Buffer::insertData - " << gluErrorString( glGetError() ) << std::endl;
}


/***
 * 5. Auxiliar methods
 ***/

void Buffer::clear()
{
    occupiedSize = 0;
}

} // namespace gpu_buf
