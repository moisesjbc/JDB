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

#include "tileset.hpp"
#include <iostream>

namespace m2g {


/***
 * 1. Initialization and destruction.
 ***/

TilesetsBuffer* Tileset::tilesetsBuffer = nullptr;
unsigned int Tileset::refCount = 0;


Tileset::Tileset( const tinyxml2::XMLNode* xmlNode, const char* folder ) :
    texture( 0 ),
    tileWidth( 0 ),
    tileHeight( 0 ),
    imageWidth( 0 ),
    imageHeight( 0 ),
    nRows( 0 ),
    nColumns( 0 ),
    nTiles( 0 ),
    bufferIndex( 0 )
{
    if( tilesetsBuffer == nullptr ){
        // If the tilesets buffer is not initialized, create it!.
        tilesetsBuffer = new TilesetsBuffer( 10 );
    }

    // Increment the references count.
    refCount++;

    load( xmlNode, folder );
}

Tileset::~Tileset()
{
    // Free the OpenGL texture object.
    glDeleteTextures( 1, &texture );

    // Decrement the references count.
    refCount--;

    // If the references count reaches 0, delete the tilesets buffer.
    if( !refCount ){
        delete tilesetsBuffer;
        tilesetsBuffer = nullptr;
    }
}


/***
 * 2. Loading
 ***/

void Tileset::load( const tinyxml2::XMLNode* xmlNode, const char* folder )
{
    std::string tilesStr;
    unsigned int firstTile, lastTile;
    int separatorIndex;

    // Copy the tileset's name ().
    name = xmlNode->FirstChildElement( "src" )->GetText();

    // Read texture's source and frame dimensions from the given XML node.
    const char* imageFile = xmlNode->FirstChildElement( "src" )->GetText();
    const tinyxml2::XMLElement* tileDimensionsNode = xmlNode->FirstChildElement( "tile_dimensions" );
    tileWidth = (GLuint)tileDimensionsNode->IntAttribute( "width" );
    tileHeight = (GLuint)tileDimensionsNode->IntAttribute( "height" );
    const tinyxml2::XMLElement* collisionInfoNode = xmlNode->FirstChildElement( "collision_rects" );
    const tinyxml2::XMLElement* collisionRectNode;
    Rect colRect;

    SDL_Surface* image = NULL;

    // Load the texture image
    if( folder != nullptr ){
        image = IMG_Load( ( std::string( folder ) + std::string( imageFile ) ).c_str() );
    }else{
        image = IMG_Load( imageFile );
    }
    if( !image ){
        throw std::runtime_error( std::string( "ERROR: couldn't load texture image - " ) + std::string( IMG_GetError() ) );
    }
    std::cout << "Image loaded: " << imageFile << std::endl;

    // Validity condition: the tileset dimensions must be multiples
    // of the tile's dimensions.
    if( ( image->w % tileWidth ) || ( image->h % tileHeight ) ){
        throw std::runtime_error( "ERROR: Image's' dimensions are not multiples of tile's dimensions" );
    }

    // Read tileset general info.
    imageWidth = image->w;
    imageHeight = image->h;
    nRows = ( image->h / tileHeight );
    nColumns = ( image->w / tileWidth );
    nTiles = nRows * nColumns;

    // Insert the tileset vertex attributes in the tilesetsBuffer and get its index.
    bufferIndex = tilesetsBuffer->insertTileset( tileWidth, tileHeight );

    // Generate the texture and set its parameters.
    // TODO: play with multiple texture units (or not?).
    glActiveTexture( GL_TEXTURE0 );
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D_ARRAY, texture );
    std::cout << "Sprite::loadTileset - texture: " << texture << std::endl;

    glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );


    // Set the texture's storage.
    glTexStorage3D( GL_TEXTURE_2D_ARRAY,    // target
                    1,                      // levels (1 = no mipmapping).
                    GL_RGBA8,               // internal format (32-bit textures).
                    tileWidth,     // texture width.
                    tileHeight,    // texture height.
                    nTiles         // texture depth (number of slices).
                    );
    std::cout << "glTexStorage3D: " << gluErrorString( glGetError() ) << std::endl;

    // Set texture's image data.
    GLuint tile = 0;
    for( GLuint row = 0; row < nRows; row++ ){
        for( GLuint column = 0; column < nColumns; column++ ){
            glPixelStorei( GL_UNPACK_SKIP_PIXELS, column*tileWidth );
            glPixelStorei( GL_UNPACK_SKIP_ROWS, row*tileHeight );
            glPixelStorei( GL_UNPACK_ROW_LENGTH, image->w );

            glTexSubImage3D( GL_TEXTURE_2D_ARRAY,   // target
                             0,                     // level
                             0,                     // xoffset
                             0,                     // yoffset
                             tile,                  // zoffset
                             tileWidth,    // width
                             tileHeight,   // height
                             1,                     // depth
                             GL_RGBA,               // format
                             GL_UNSIGNED_BYTE,      // type
                             image->pixels          // image data.
                             );
            tile++;
        }

    }

    std::cout << "glTextSubImage3D: " << gluErrorString( glGetError() ) << std::endl;
    glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );

    // Free the image's surface.
    SDL_FreeSurface( image );

    // Create an empty vector of collision rects for each tile in the tileset.
    collisionRects.resize( nTiles );

    // Check if there is available collision info.
    if( collisionInfoNode ){

        // Get the first collision rect node.
        collisionRectNode = collisionInfoNode->FirstChildElement( "collision_rect" );

        // Process each collision rect in the tileset XML element.
        while( collisionRectNode ){

            // Get the current collision rect from the XML element.
            colRect.x = (GLfloat)( collisionRectNode->FloatAttribute( "x" ) );
            colRect.y = (GLfloat)( collisionRectNode->FloatAttribute( "y" ) );
            colRect.width = (GLfloat)( collisionRectNode->FloatAttribute( "width" ) );
            colRect.height = (GLfloat)( collisionRectNode->FloatAttribute( "height" ) );

            // The XML attribute "tiles" tells us in how many tiles is
            // the current collision rect present.
            tilesStr = collisionRectNode->Attribute( "tiles" );

            if( tilesStr == "all" ){
                // The collision rect is present in all the tiles
                // of the tileset.
                for( unsigned int i=0; i<nTiles; i++ ){
                    collisionRects[i].push_back( colRect );
                }
            }else{
                // The collision rect is present in a subset of
                // tiles.
                separatorIndex = tilesStr.find( '-' );
                if( separatorIndex != -1 ){
                    // The collision rect is present in a range
                    // of tiles [first-last].
                    firstTile = atoi( tilesStr.substr( 0, separatorIndex ).c_str() );
                    lastTile = atoi( tilesStr.substr( separatorIndex+1 ).c_str() );
                }else{
                    // The collision rect is present in an individual
                    // tile.
                    firstTile = atoi( tilesStr.c_str() );
                    lastTile = atoi( tilesStr.c_str() );
                }

                // Add the collision rect to the requested tiles' collision
                // info.
                for( unsigned int i=firstTile; i<=lastTile; i++ ){
                    collisionRects[i].push_back( colRect );
                }
            }

            // Get the next collision rect XML node.
            collisionRectNode = collisionRectNode->NextSiblingElement( "collision_rect" );
        }
    }
}


/***
 * 3. Setters and getters
 ***/

GLuint Tileset::getTexture() const
{
    return texture;
}


GLuint Tileset::getNTiles() const
{
    return nTiles;
}


std::string Tileset::getName() const
{
    return name;
}


const std::vector< Rect >* Tileset::getCollisionRects( unsigned int tile ) const
{
    return &collisionRects[tile];
}


/***
 * 4. Drawing
 ***/

void Tileset::draw() const
{
    tilesetsBuffer->draw( bufferIndex );
}


/***
 * 5. Auxiliar methods
 ***/

void Tileset::bindBuffer()
{
    tilesetsBuffer->bind();
}

} // Namespace m2g
