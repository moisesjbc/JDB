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

    GLfloat vertices[] {
        // Bottom left
        0.0f, 0.0f,     // Vertice coordinates
        0.0f, 0.0f,     // Texture coordinates

        // Top left
        0.0f, height,   // Vertice coordinates
        0.0f, 1.0f,     // Texture coordinates

        // Bottom right
        width, 0.0f,   // Vertice coordinates
        1.0f, 0.0f,     // Texture coordinates

        // Top right
        width, height,  // Vertice coordinates
        1.0f, 1.0f     // Texture coordinates
    };

    // Generate a VBO and fill it with Sprite's vertex attributes.
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, 16*sizeof( GLfloat ), vertices, GL_STATIC_DRAW );

    // If we still don't have the locations of the shader uniforms, we
    // search them here.
    if( mvpMatrixLocation == -1 ){
        glGetIntegerv( GL_CURRENT_PROGRAM, &currentProgram );

        mvpMatrixLocation = glGetUniformLocation( currentProgram, "mvpMatrix" );
        samplerLocation = glGetUniformLocation( currentProgram, "sampler" );
    }

    std::cout << "Sprite - Vertices and locations loaded: " << gluErrorString( glGetError() ) << std::endl;

    // Generate a 2D texture id.
    glActiveTexture( GL_TEXTURE0 );
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );

    // Set texture's wrapping and filtering parameters.
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    // Get a pointer to the OpenGL function glTexStorage2D.
    typedef void (APIENTRY * GL_TexStorage2D_Func)(GLenum, GLsizei, GLenum, GLsizei, GLsizei );
    GL_TexStorage2D_Func glTexStorage2D = NULL;
    glTexStorage2D = (GL_TexStorage2D_Func)SDL_GL_GetProcAddress( "glTexStorage2D" );

    // Set the texture's storage.
    glTexStorage2D( GL_TEXTURE_2D, 1, GL_RGBA8UI, width, height );

    //glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    //glPixelStorei(GL_PACK_ALIGNMENT, 4);

    // Declare a array with the texture's image data.
    unsigned int size = width * height;
    GLuint imageData[size];
    std::cout << "size: " << size << std::endl
         << "size/4: " << (size/4) << std::endl
         << "size/2: " << (size/2) << std::endl
         << "3/4*size: " << (size*(3.0f/4.0f)) << std::endl;

    // Intialize the image data.
    unsigned int i;
    // Red
    //for( i=0; i<size/4; i++ ){
    for( i=0; i<size/2; i++ ){
        imageData[i] = 0x000000FF;
    }

    // Blue
    for( ; i<size; i++ ){
        imageData[i] = 0x0000FF00FF;
    }

    /*
    // Green
    for( ; i<size*(3.0f/4.0f); i++ ){
        imageData[i] = 0x0000FFFF;
    }

    // White
    for( ; i<size; i++ ){
        imageData[i] = 0xFFFFFFFF;
    }
    */

    // Set texture's image data.
    glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, imageData );
    std::cout << "glTextSubImage: " << gluErrorString( glGetError() ) << std::endl;

    // Connect sampler to texture unit.
    glUniform1i( samplerLocation, 0 );
    std::cout << "Location of shader variable \"sampler\" set : " << gluErrorString( glGetError() ) << std::endl;

    // If the VAO for sprites hasn't been initialized... well, it is time.
    if( !vao ){
        Sprite::initializeVAO();
    }

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
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // We are sending 2D vertices to the vertex shader.
    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*)(0) );
    glEnableVertexAttribArray( 0 );

    // We are sending 2D texture coordinates to the shader.
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*)(2*sizeof(GLfloat)) );
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
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
}


/***
 * 4. Auxiliar methods
 ***/

void Sprite::sendMVPMatrixToShader( const glm::mat4& mvpMatrix ) const
{
    glUniformMatrix4fv( mvpMatrixLocation, 1, GL_FALSE, &mvpMatrix[0][0] );
}


} // Namespace jdb
