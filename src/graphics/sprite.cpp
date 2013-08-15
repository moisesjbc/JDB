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
GLint Sprite::samplerLocation = -1;

/***
 * 1. Initialization
 ***/

Sprite::Sprite( )
{
    GLint currentProgram;

    SDL_Surface* image = NULL;

    const GLfloat width = 256;
    const GLfloat height = 64;

    GLfloat vertices[] {
        // Bottom left
        0.0f, 0.0f,     // Vertice coordinates
        0.0f, 0.0f,     // Texture coordinates

        // Bottom right
        width, 0.0f,    // Vertice coordinates
        1.0f, 0.0f,     // Texture coordinates

        // Top right
        width, height,  // Vertice coordinates
        1.0f, 1.0f,     // Texture coordinates

        // Top left
        0.0f, height,   // Vertice coordinates
        0.0f, 1.0f      // Texture coordinates
    };



    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, 16*sizeof( GLfloat ), vertices, GL_STATIC_DRAW );

    image = IMG_Load( "data/img/sandwich_01.png" );

    if( !image ){
        std::cout << SDL_GetError() << std::endl;
    }
    std::cout << "image: " << image << std::endl;
    //
    glGenTextures( 1, &tbo );
    glBindTexture( GL_TEXTURE_2D, tbo );
    //glTexStorage2D( GL_TEXTURE_2D, 1, GL_RGBA8, image->w, image->h );
    //glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, image->w, image->h, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels );
    SDL_FreeSurface( image );

    GLuint sampler;

    glGenSamplers( 1, &sampler );
    glBindSampler( tbo, sampler );

    gluErrorString( glGetError() );

    // If the VAO for sprites hasn't been initialized... well, initialize it is time.
    if( !vao ){
        Sprite::initializeVAO();
    }

    // If we still don't have the locations of the shader uniforms, we
    // search them here.
    if( mvpMatrixLocation == -1 ){
        glGetIntegerv( GL_CURRENT_PROGRAM, &currentProgram );

        mvpMatrixLocation = glGetUniformLocation( currentProgram, "mvpMatrix" );
        samplerLocation = glGetUniformLocation( currentProgram, "sampler" );
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

    // We are sending 2D texture coordinates to the shader.
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), (GLvoid*)(2*sizeof(GLfloat)) );
    glEnableVertexAttribArray( 1 );
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

    glBindTexture( GL_TEXTURE_2D, tbo );

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
