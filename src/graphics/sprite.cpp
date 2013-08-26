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

    const GLfloat width = 32.0f;
    const GLfloat height = 32.0f;

    GLfloat vertices[] {
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

        currentTile = 0;
    }

    // Connect sampler to texture unit 0.
    glUniform1i( samplerLocation, 0 );
    std::cout << "Location of shader variable \"sampler\" set : " << gluErrorString( glGetError() ) << std::endl;

    // If the VAO for sprites hasn't been initialized... well, it is time.
    if( !vao ){
        Sprite::initializeVAO();
    }

    std::cout << "Sprite - VAO initialized: " << gluErrorString( glGetError() ) << std::endl;
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
    GLsizei nRows, nColumns;

    SDL_Surface* image = NULL;

    // Load the texture image
    image = IMG_Load( imageFile );
    if( !image ){
        throw std::runtime_error( std::string( "ERROR: couldn't load texture image - " ) + std::string( IMG_GetError() ) );
    }
    std::cout << "Image loaded: " << imageFile << std::endl;


    if( ( image->w % tileWidth ) || ( image->h % tileHeight ) ){
        throw std::runtime_error( "ERROR: Image's' dimensions are not multiples of tile's dimensions" );
    }
    nRows = ( image->h / tileHeight );
    nColumns = ( image->w / tileWidth );
    tileset->nTiles = nRows * nColumns;

    std::cout << "Tileset: columns (" << nColumns << ")    rows (" << nRows << ")" << std::endl;

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
                    tileset->nTiles         // texture depth (number of slices).
                    );
    std::cout << "glTexStorage3D: " << gluErrorString( glGetError() ) << std::endl;

    // Set texture's image data.
    GLsizei tile = 0;
    for( GLsizei row = 0; row < nRows; row++ ){
        for( GLsizei column = 0; column < nColumns; column++ ){
            glPixelStorei( GL_UNPACK_SKIP_PIXELS, column*tileWidth );
            glPixelStorei( GL_UNPACK_SKIP_ROWS, row*tileHeight );
            glPixelStorei( GL_UNPACK_ROW_LENGTH, image->w );

            glTexSubImage3D( GL_TEXTURE_2D_ARRAY,   // target
                             0,                     // level
                             0,                     // xoffset
                             0,                     // yoffset
                             tile,                  // zoffset
                             tileWidth,             // width
                             tileHeight,            // height
                             1,                     // depth
                             GL_RGBA,               // format
                             GL_UNSIGNED_BYTE,      // type
                             image->pixels          // image data.
                             );
            tile++;
        }

    }

    std::cout << "glTextSubImage3D: " << gluErrorString( glGetError() ) << std::endl;

    // Free the image's surface.
    SDL_FreeSurface( image );

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
 * 3. Current tile management
 ***/

void Sprite::nextTile()
{
    if( currentTile < (tileset->nTiles - 1) ){
        currentTile++;
    }else{
        currentTile = 0;
    }
}

void Sprite::previousTile()
{
    if( currentTile ){
        currentTile--;
    }else{
        currentTile = tileset->nTiles - 1;
    }
}

void Sprite::setTile( const GLuint tile )
{
    if( tile < tileset->nTiles ){
        currentTile = tile;
    }else{
        throw std::runtime_error( "ERROR: tile index out of limits" );
    }
}


/***
 * 4. Drawing
 ***/

void Sprite::draw( const glm::mat4& projectionMatrix ) const {
    // Set this Sprite's VBO as the active one.
    glActiveTexture( GL_TEXTURE0 );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBindTexture( GL_TEXTURE_2D_ARRAY, tileset->texture );

    // Send current tile index to shader.
    glUniform1ui( sliceLocation, currentTile );

    // Send MVP matrix to shader.
    sendMVPMatrixToShader( projectionMatrix * transformationMatrix );

    // Draw the sprite.
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
}


/***
 * 5. Auxiliar methods
 ***/

void Sprite::sendMVPMatrixToShader( const glm::mat4& mvpMatrix ) const
{
    glUniformMatrix4fv( mvpMatrixLocation, 1, GL_FALSE, &mvpMatrix[0][0] );
}


} // Namespace jdb
