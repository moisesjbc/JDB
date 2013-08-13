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

#include "sprite.hpp"
#include <iostream>

namespace jdb {

GLuint Sprite::vao = 0;
GLint Sprite::mvpMatrixLocation = -1;

/***
 * 1. Initialization
 ***/

Sprite::Sprite( )
{
    GLint currentProgram;

    const GLfloat size = 100.0f;
    GLfloat vertices[] {
        0, 0,       // Bottom left
        size, 0,    // Bottom right
        size, size, // Top right
        0, size     // Top left
    };

    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, 8*sizeof( GLfloat ), vertices, GL_STATIC_DRAW );

    // If the VAO for sprites hasn't been initialized... well, initialize it is time.
    if( !vao ){
        Sprite::initializeVAO();
    }

    // If we still don't have the location of the shader uniform "mvpMatrix", we
    // search it here.
    if( mvpMatrixLocation == -1 ){
        glGetIntegerv( GL_CURRENT_PROGRAM, &currentProgram );

        mvpMatrixLocation = glGetUniformLocation( currentProgram, "mvpMatrix" );
    }
}

void Sprite::setSpriteData( const std::shared_ptr< SpriteData >& spriteData )
{
    this->spriteData = spriteData;
}


/***
 * 2. VAO management
 ***/

void Sprite::initializeVAO()
{
    // Create a VAO for sprites.
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // We are sending 2D vertices to the vertex shader.
    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( 0 );
}

GLuint Sprite::getVAO()
{
    return vao;
}


/***
 * 3. Drawing
 ***/

void Sprite::draw( const glm::mat4& projectionMatrix ) const {
    // Set this Sprite's VBO as the active one.
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    // Send MVP matrix to shader.
    // TODO: change and use the Sprite's own transformation matrix.
    sendMVPMatrixToShader( projectionMatrix * glm::translate( glm::mat4( 1.0f ), glm::vec3( 400.0f, 100.0f, 0.0f ) ) );

    // Draw the sprite.
    glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
}


/***
 * 4. Auxiliar methods
 ***/

void Sprite::sendMVPMatrixToShader( const glm::mat4& mvpMatrix ) const
{
    glUniformMatrix4fv( mvpMatrixLocation, 1, GL_FALSE, &mvpMatrix[0][0] );
}


} // Namespace jdb
