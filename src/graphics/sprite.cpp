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

    const GLfloat width = 64.0f;
    const GLfloat height = 64.0f;

    const GLsizei textureSize = 16;

    GLfloat vertices[] {
        // Vertice coordinates
        0.0f, height,   // Bottom left
        width, height,  // Bottom right
        0.0f, 0.0f,     // Top left
        width, 0.0f,    // Top right

        // Texture coordinates
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
    };

    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Generate a VBO and fill it with Sprite's vertex attributes.
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, 16*sizeof( GLfloat ), vertices, GL_STATIC_DRAW );
    std::cout << "Sprite - Vertices and locations loaded 1: " << gluErrorString( glGetError() ) << std::endl;

    // If we still don't have the locations of the shader uniforms, we
    // search them here.
    if( mvpMatrixLocation == -1 ){
        glGetIntegerv( GL_CURRENT_PROGRAM, &currentProgram );

        mvpMatrixLocation = glGetUniformLocation( currentProgram, "mvpMatrix" );
        samplerLocation = glGetUniformLocation( currentProgram, "tex" );

        std::cout << "mvpMatrixLocation: " << mvpMatrixLocation << std::endl
                  << "samplerLocation: " << samplerLocation << std::endl;
    }

    std::cout << "Sprite - Vertices and locations loaded: " << gluErrorString( glGetError() ) << std::endl;

    // Generate a 2D texture id.
    glActiveTexture( GL_TEXTURE0 );
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );

    std::cout << "texture: " << texture << std::endl;

    // Set texture's wrapping and filtering parameters.
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    //glPixelStorei( GL_PACK_ALIGNMENT, 1 );

    // Set the texture's storage.
    glTexStorage2D( GL_TEXTURE_2D, 1, GL_RGBA32F, textureSize, textureSize );
    std::cout << "glTexStorage2D: " << gluErrorString( glGetError() ) << std::endl;

    // Declare a array with the texture's image data.
    const unsigned int size = 4*2*textureSize;
    GLubyte imageData[size];
    std::cout << "size: " << size << std::endl
         << "size/4: " << (size/4) << std::endl
         << "size/2: " << (size/2) << std::endl
         << "3/4*size: " << (size*(3.0f/4.0f)) << std::endl;

    // Intialize the image data.
    unsigned int i;
    // Red
    //for( i=0; i<size/4; i++ ){
    for( i=0; i<size; i+=4 ){
        imageData[i] = 255;
        imageData[i+1] = 0;
        imageData[i+2] = 0;
        imageData[i+3] = 255;
    }

    // Blue
    /*for( ; i<size; i+=4 ){
        //std::cout << "i (2) : " << i << std::endl;
        imageData[i] = 0;
        imageData[i+1] = 255;
        imageData[i+2] = 0;
        imageData[i+3] = 0;
    }*/

    std::cout << "Loading image data into texture ..." << std::endl;
    // Set texture's image data.
    glTexSubImage2D( GL_TEXTURE_2D,     // target
                     0,                 // level
                     0,                 // xoffset
                     0,                 // yoffset
                     textureSize,       // width
                     textureSize,       // height
                     GL_RGBA,           // format
                     GL_UNSIGNED_BYTE,  // type
                     imageData          // image data.
                     );
    std::cout << "glTextSubImage2D: " << gluErrorString( glGetError() ) << std::endl;

    // Connect sampler to texture unit.
    glUniform1i( samplerLocation, 0 );
    std::cout << "Location of shader variable \"sampler\" set : " << gluErrorString( glGetError() ) << std::endl;

    // If the VAO for sprites hasn't been initialized... well, it is time.
    //if( !vao ){
        Sprite::initializeVAO();
    //}

    std::cout << "Sprite - VAO initialized: " << gluErrorString( glGetError() ) << std::endl;
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
    // Initialize the VAO shared by all sprites.


    // We are sending 2D vertices to the vertex shader.
    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0) );
    glEnableVertexAttribArray( 0 );

    // We are sending 2D texture coordinates to the shader.
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(8*sizeof(GLfloat)) );
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
    glActiveTexture( GL_TEXTURE0 );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBindTexture( GL_TEXTURE_2D, texture );

    // Send MVP matrix to shader.
    // TODO: change and use the Sprite's own transformation matrix.
    sendMVPMatrixToShader( projectionMatrix * glm::translate( glm::mat4( 1.0f ), glm::vec3( 400.0f, 100.0f, 0.0f ) ) );

    // Draw the sprite.
    glDrawArrays( GL_TRIANGLES, 0, 3 );
}


/***
 * 4. Auxiliar methods
 ***/

void Sprite::sendMVPMatrixToShader( const glm::mat4& mvpMatrix ) const
{
    glUniformMatrix4fv( mvpMatrixLocation, 1, GL_FALSE, &mvpMatrix[0][0] );
}


} // Namespace jdb
