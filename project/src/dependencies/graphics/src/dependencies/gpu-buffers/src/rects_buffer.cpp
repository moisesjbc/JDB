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

#include "rects_buffer.hpp"
#include <iostream>

namespace gpu_buf {

const GLuint VERTICES_PER_RECT = 4;
const GLuint COORDINATES_PER_VERTICE = 2;
const GLuint RECT_VERTEX_DATA_SIZE = VERTICES_PER_RECT * COORDINATES_PER_VERTICE * sizeof( GLfloat );

RectsBuffer::RectsBuffer( GLuint nRects, GLuint shaderVertexLocation ) :
    EntitiesBuffer( GL_ARRAY_BUFFER, GL_STATIC_DRAW, nRects * RECT_VERTEX_DATA_SIZE, 4 ),
    nRects( 0 )
{
    // We are sending 2D vertices to the vertex shader.
    setVertexAttribPointer( shaderVertexLocation, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0) );

    glDisableVertexAttribArray( 1 );
    glVertexAttrib2f( 1, 0.0f, 0.0f );

    std::cout << "Rects Buffer constructor - " << gluErrorString( glGetError() ) << std::endl;
}


/***
 * 2. Data insertion
 ***/

unsigned int RectsBuffer::insertRect( GLfloat x, GLfloat y, GLfloat w, GLfloat h )
{
    GLfloat vertexData[8];

    // Bottom left
    vertexData[0] = x;
    vertexData[1] = y;

    // Bottom right
    vertexData[2] = x + w;
    vertexData[3] = y;

    // Top right
    vertexData[4] = x + w;
    vertexData[5] = y + h;

    // Top left
    vertexData[6] = x;
    vertexData[7] = y + h;

    // Insert data into buffer.
    insertData( vertexData, 8*sizeof(GLfloat) );
    nRects++;

    // Return the index of the recently added rect / entity. We get it by dividing the
    // occupied size (in bytes) by 8 (4 * 2D vertices per rect) multiplied by the sizeof
    // of a GLfloat.
    return ( occupiedSize / ( 8 * sizeof( GLfloat ) ) ) - 1;
}


/***
 * 3. Drawing
 ***/

void RectsBuffer::drawAll( GLfloat* color )
{
    // If not color was defined, use the Buffer's default one.
    if( color == nullptr ){
        color = defaultColor;
    }

    // Send the color to the uniform.
    glUniform4fv( colorUniformLocation, 1, color );

    // Bind the buffer's VAO and VBO as the active ones.
    glBindVertexArray( vao );
    glBindBuffer( vboTarget, vbo );

    // Draw all the rects in the buffer.
    for( unsigned int i=0; i<nRects*4; i+=4 ){
        glDrawArrays( GL_LINE_LOOP, i, 4 );
    }
}


void RectsBuffer::drawEntity( GLuint index, GLfloat* color )
{
    // Bind the buffer's VAO and VBO as the active ones.
    glBindVertexArray( vao );
    glBindBuffer( vboTarget, vbo );

    // If not color was defined, use the Buffer's default one.
    if( color == nullptr ){
        color = defaultColor;
    }

    // Send the color to the uniform.
    glUniform4fv( colorUniformLocation, 1, color );

    // Draw the requested rect.
    glDrawArrays( GL_LINE_LOOP, index*VERTICES_PER_ENTITY, VERTICES_PER_ENTITY );
}


/***
 * 4. Auxiliar methods
 ***/

void RectsBuffer::clear()
{
    Buffer::clear();
    nRects = 0;
}

} // namespace gpu_buf
