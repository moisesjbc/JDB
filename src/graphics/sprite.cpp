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
GLint Sprite::sliceLocation = -1;

/***
 * 1. Initialization
 ***/

Sprite::Sprite( )
{
    GLint currentProgram;

    const GLfloat width = 256.0f;
    const GLfloat height = 64.0f;

    const GLsizei textureWidth = 256;
    const GLsizei textureHeight = 256;
    const unsigned int textureSize = 4*textureHeight*textureWidth;

    SDL_Surface* image;

    GLfloat vertices[] {
        // Vertice coordinates
        0.0f, 0.0f,     // Bottom left
        width, 0.0f,    // Bottom right
        0.0f, height,   // Top left
        width, height,  // Top right

        // Texture coordinates
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f
    };

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
        sliceLocation = glGetUniformLocation( currentProgram, "slice" );

        std::cout << "mvpMatrixLocation: " << mvpMatrixLocation << std::endl
                  << "samplerLocation: " << samplerLocation << std::endl
                  << "sliceLocation: " << sliceLocation << std::endl;

        glUniform1i( sliceLocation, 0 );
    }

    glActiveTexture( GL_TEXTURE0 );
    /*
    std::cout << "Sprite - Vertices and locations loaded: " << gluErrorString( glGetError() ) << std::endl;

    // Generate a 2D texture id.
    glActiveTexture( GL_TEXTURE0 );
    glGenTextures( 1, &texture );
    std::cout << "texture: " << texture << std::endl;
    glBindTexture( GL_TEXTURE_2D, texture );

    // Set texture's wrapping and filtering parameters.
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    //glPixelStorei( GL_PACK_ALIGNMENT, 1 );

    // Set the texture's storage.
    glTexStorage2D( GL_TEXTURE_2D,  // target
                    1,              // levels (1 = no mipmapping)
                    GL_RGBA8,       // internal format (32-bit textures)
                    image->w,       // texture width
                    image->h        // texture height
                    );
    std::cout << "glTexStorage2D: " << gluErrorString( glGetError() ) << std::endl;

    // Declare a array with the texture's image data.
    GLubyte imageData[textureSize];

    // Intialize the image data.
    unsigned int i;
    // Red
    //for( i=0; i<size/4; i++ ){
    for( i=0; i<textureSize/2; i+=4 ){
        imageData[i] = (GLubyte)255;
        imageData[i+1] = (GLubyte)0;
        imageData[i+2] = (GLubyte)0;
        imageData[i+3] = (GLubyte)255;
    }
    for( ; i<textureSize; i+=4 ){
        imageData[i] = (GLubyte)0;
        imageData[i+1] = (GLubyte)255;
        imageData[i+2] = (GLubyte)0;
        imageData[i+3] = (GLubyte)255;
    }

    imageData[0] = (GLubyte)0;
    imageData[1] = (GLubyte)0;
    imageData[2] = (GLubyte)255;
    imageData[3] = (GLubyte)255;

    std::cout << "Loading image data into texture ..." << std::endl;
    // Set texture's image data.
    glTexSubImage2D( GL_TEXTURE_2D,     // target
                     0,                 // level
                     0,                 // xoffset
                     0,                 // yoffset
                     image->w,          // width
                     image->h,          // height
                     GL_RGBA,           // format
                     GL_UNSIGNED_BYTE,  // type
                     image->pixels      // image data.
                     );
    std::cout << "glTextSubImage2D: " << gluErrorString( glGetError() ) << std::endl;
    */
    // Connect sampler to texture unit.
    glUniform1i( samplerLocation, 0 );
    std::cout << "Location of shader variable \"sampler\" set : " << gluErrorString( glGetError() ) << std::endl;

    // If the VAO for sprites hasn't been initialized... well, it is time.
    //if( !vao ){
        Sprite::initializeVAO();
    //}

    std::cout << "Sprite - VAO initialized: " << gluErrorString( glGetError() ) << std::endl;

    //SDL_FreeSurface( image );
}

void Sprite::setTileset( const std::shared_ptr< Tileset >& tileset )
{
    this->tileset = tileset;

    std::cout << "Sprite::setTileset - tileset->texture: " << tileset->texture << std::endl;
}


std::shared_ptr<Tileset> Sprite::loadTileset( const tinyxml2::XMLNode* xmlNode )
{
    std::shared_ptr<Tileset> tileset( new Tileset );

    // Read texture's source and frame dimensions from the given XML node.
    const char* imageFile = xmlNode->FirstChildElement( "src" )->GetText();
    const tinyxml2::XMLElement* tileDimensionsNode = xmlNode->FirstChildElement( "tile_dimensions" );
    const GLsizei tileWidth = (GLsizei)tileDimensionsNode->IntAttribute( "width" );
    const GLsizei tileHeight = (GLsizei)tileDimensionsNode->IntAttribute( "height" );

    SDL_Surface* image = NULL;

    // Load the texture image
    image = IMG_Load( imageFile );
    if( !image ){
        throw std::runtime_error( std::string( "ERROR: couldn't load texture image - " ) + std::string( IMG_GetError() ) );
    }

    std::cout << "Image loaded: " << imageFile << std::endl;

    // Generate the texture and set its parameters.
    // TODO: play with multiple texture units (or not?).
    glActiveTexture( GL_TEXTURE0 );
    glGenTextures( 1, &tileset->texture );
    glBindTexture( GL_TEXTURE_2D_ARRAY, tileset->texture );
    std::cout << "Sprite::loadTileset - texture: " << tileset->texture << std::endl;

    glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    // Set the texture's storage.
    glTexStorage3D( GL_TEXTURE_2D_ARRAY,    // target
                    1,                      // levels (1 = no mipmapping).
                    GL_RGBA8,               // internal format (32-bit textures).
                    tileWidth,              // texture width.
                    tileHeight,             // texture height.
                    1                       // texture depth (number of slices).
                    );
    std::cout << "glTexStorage3D: " << gluErrorString( glGetError() ) << std::endl;

    // Set texture's image data.
    // TODO: extend to multiple layers.
    glTexSubImage3D( GL_TEXTURE_2D_ARRAY,   // target
                     0,                     // level
                     0,                     // xoffset
                     0,                     // yoffset
                     0,                     // zoffet
                     tileWidth,             // width
                     tileHeight,            // height
                     1,                     // depth
                     GL_RGBA,               // format
                     GL_UNSIGNED_BYTE,      // type
                     image->pixels          // image data.
                     );
    std::cout << "glTextSubImage3D: " << gluErrorString( glGetError() ) << std::endl;

    // Free the image's surface.
    SDL_FreeSurface( image );

    //glBindTexture( GL_TEXTURE_2D_ARRAY, 0 );

    return tileset;
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
    glBindTexture( GL_TEXTURE_2D_ARRAY, tileset->texture );

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
