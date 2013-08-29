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

GLint Sprite::mvpMatrixLocation = -1;
GLint Sprite::samplerLocation = -1;
GLint Sprite::sliceLocation = -1;

/***
 * 1. Initialization
 ***/

Sprite::Sprite()
{
    GLint currentProgram;

    // Generate a id for the Sprite's VAO.
    glGenVertexArrays( 1, &vao );

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
}

void Sprite::setTileset( const std::shared_ptr< Tileset >& tileset )
{
    this->tileset = tileset;

    // Initialize the VAO.
    Sprite::initializeVAO();

    std::cout << "Sprite - texture and VAO initialized: " << gluErrorString( glGetError() ) << std::endl;
}


Sprite::~Sprite()
{
    glDeleteVertexArrays( 1, &vao );
}

std::shared_ptr<Tileset> Sprite::loadTileset( const tinyxml2::XMLNode* xmlNode )
{
    std::shared_ptr<Tileset> tileset( new Tileset );

    const GLfloat width = 1.0f;
    const GLfloat height = 1.0f;

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


    // Read texture's source and frame dimensions from the given XML node.
    const char* imageFile = xmlNode->FirstChildElement( "src" )->GetText();
    const tinyxml2::XMLElement* tileDimensionsNode = xmlNode->FirstChildElement( "tile_dimensions" );
    const GLsizei tileWidth = (GLsizei)tileDimensionsNode->IntAttribute( "width" );
    const GLsizei tileHeight = (GLsizei)tileDimensionsNode->IntAttribute( "height" );
    const tinyxml2::XMLElement* collisionInfoNode = xmlNode->FirstChildElement( "collision_info" );
    const tinyxml2::XMLElement* collisionRectNode;
    GLsizei nRows, nColumns;
    Rect colRect;

    // Update the previous array "vertices" with the width and the height
    // of a tile.
    vertices[4] = tileWidth;
    vertices[12] = tileWidth;
    vertices[9] = tileHeight;
    vertices[13] = tileHeight;

    // Generate a VBO and fill it with previous array "vertices".
    glGenBuffers( 1, &(tileset->vbo) );
    glBindBuffer( GL_ARRAY_BUFFER, tileset->vbo );
    glBufferData( GL_ARRAY_BUFFER, 16*sizeof( GLfloat ), vertices, GL_STATIC_DRAW );
    std::cout << "Tileset - Vertices loaded: " << gluErrorString( glGetError() ) << std::endl;

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


    // Get collision info (if available).
    if( collisionInfoNode ){
        collisionRectNode = collisionInfoNode->FirstChildElement( "general_col_rects" )->FirstChildElement( "rect" );

        while( collisionRectNode ){
            colRect.x = (GLfloat)( collisionRectNode->FloatAttribute( "x" ) );
            colRect.y = (GLfloat)( collisionRectNode->FloatAttribute( "y" ) );
            colRect.width = (GLfloat)( collisionRectNode->FloatAttribute( "width" ) );
            colRect.height = (GLfloat)( collisionRectNode->FloatAttribute( "height" ) );

            std::cout << "colRect: (" << colRect.x << ", " << colRect.y << ", " << colRect.width << ", " << colRect.height << ")" << std::endl;

            tileset->generalCollisionRects.push_back( colRect );

            collisionRectNode = collisionRectNode->NextSiblingElement();
        }
    }

    return tileset;
}

const std::shared_ptr< Tileset > Sprite::getTileset()
{
    return tileset;
}


glm::vec2 Sprite::getPosition() const
{
    return position;
}

/***
 * 2. VAO management
 ***/

void Sprite::initializeVAO()
{
    // Initialize the sprite's VAO.
    glBindVertexArray( vao );

    // We are sending 2D vertices to the vertex shader.
    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*)(0) );
    glEnableVertexAttribArray( 0 );

    // We are sending 2D texture coordinates to the shader.
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*)(2*sizeof(GLfloat)) );
    glEnableVertexAttribArray( 1 );
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


GLuint Sprite::getCurrentTile() const
{
    return currentTile;
}


/***
 * 4. Collision detection
 ***/

bool Sprite::collide( const Sprite& b ) const
{
    const std::vector<Rect>* aRects = getCollisionRects();
    const std::vector<Rect>* bRects = b.getCollisionRects();

    const glm::vec2 bPosition = b.getPosition();

    Rect aRect, bRect;

    for( unsigned int i=0; i<aRects->size(); i++ ){
        aRect.x = ( (*aRects)[i] ).x + position.x;
        aRect.y = ( (*aRects)[i] ).y + position.y;
        aRect.width = ( (*aRects)[i] ).width;
        aRect.height = ( (*aRects)[i] ).height;

        for( unsigned int j=0; j<bRects->size(); j++ ){
            bRect.x = ( (*bRects)[j] ).x + bPosition.x;
            bRect.y = ( (*bRects)[j] ).y + bPosition.y;
            bRect.width = ( (*bRects)[j] ).width;
            bRect.height = ( (*bRects)[j] ).height;

            if(
                ( aRect.x < ( bRect.x + bRect.width ) ) && // 4
                ( ( aRect.x + aRect.width ) > bRect.x ) && // 3
                ( aRect.y < ( bRect.y + bRect.height ) ) && // 2
                ( ( aRect.y + aRect.height ) > bRect.y )  // 1
              ){
                return true;
            }
        }
    }

    return false;
}

const std::vector<Rect>* Sprite::getCollisionRects() const
{
    return &( tileset->generalCollisionRects );
}


/***
 * 5. Drawing
 ***/

void Sprite::draw( const glm::mat4& projectionMatrix ) const {
    // Load the sprite's attributes for rendering.
    glActiveTexture( GL_TEXTURE0 );
    glBindVertexArray( vao );
    glBindTexture( GL_TEXTURE_2D_ARRAY, tileset->texture );

    // Send current tile index to shader.
    glUniform1ui( sliceLocation, currentTile );

    // Send MVP matrix to shader.
    sendMVPMatrixToShader( projectionMatrix * glm::translate( glm::mat4( 1.0f ), glm::vec3( position.x, position.y, 0.0f ) ) );

    // Draw the sprite.
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
}


/***
 * 6. Auxiliar methods
 ***/

void Sprite::sendMVPMatrixToShader( const glm::mat4& mvpMatrix ) const
{
    glUniformMatrix4fv( mvpMatrixLocation, 1, GL_FALSE, &mvpMatrix[0][0] );
}


} // Namespace jdb
