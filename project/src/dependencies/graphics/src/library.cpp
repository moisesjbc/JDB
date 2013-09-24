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

#include "library.hpp"

namespace m2g {

/***
 * 1. Initialization and destruction
 ***/

Library::Library()
{}


/***
 * 2. File management
 ***/

void Library::loadFile( const std::string& filePath, bool developerMode )
{
    tinyxml2::XMLDocument xmlFile;
    tinyxml2::XMLNode* tilesetNode;
    std::string imagesFolder;

    // Get the path to the "default" images folder.
    // TODO: Allow other skins loading.
    imagesFolder = filePath.substr( 0, filePath.rfind( "/" ) ) + "/default/";

    // Open the requested file.
    xmlFile.LoadFile( filePath.c_str() );

    // Load the first tileset XML node.
    tilesetNode = ( xmlFile.FirstChildElement( "library" ) )->FirstChildElement();

    // Keep reading tilesets until the end of file.
    while( tilesetNode ){
        // Load the tileset in the current XML element.
        tilesets.push_back( loadTileset( tilesetNode, imagesFolder.c_str(), developerMode ) );

        // Get next XML element.
        tilesetNode = tilesetNode->NextSiblingElement();
    }
}


/***
 * 3. Setters and getters
 ***/

const std::shared_ptr< m2g::Tileset > Library::getTileset( const unsigned int& index ) const
{
    return tilesets[index];
}


const std::shared_ptr< m2g::Tileset> Library::getTileset( const std::string& imageName ) const
{
    unsigned int i = 0;

    while( ( i < tilesets.size() ) &&
           strcmp( tilesets[i]->name, imageName.c_str() ) ){
        i++;
    }

    if( i >= tilesets.size() ){
        throw std::runtime_error( "ERROR: Tileset [" + imageName + "] not found in library" );
    }

    return tilesets[i];
}


/***
 * 4. Tileset and animation data loading
 ***/

std::shared_ptr<Tileset> Library::loadTileset( const tinyxml2::XMLNode* xmlNode, const char* folder, bool developerMode )
{
    std::shared_ptr<Tileset> tileset( new Tileset );
    std::string tilesStr;
    unsigned int firstTile, lastTile;
    int separatorIndex;

    // Copy the tileset's name ().
    strncpy( tileset->name, xmlNode->FirstChildElement( "src" )->GetText(), MAX_TILESET_NAME_SIZE );

    // Read texture's source and frame dimensions from the given XML node.
    const char* imageFile = xmlNode->FirstChildElement( "src" )->GetText();
    const tinyxml2::XMLElement* tileDimensionsNode = xmlNode->FirstChildElement( "tile_dimensions" );
    tileset->tileWidth = (GLuint)tileDimensionsNode->IntAttribute( "width" );
    tileset->tileHeight = (GLuint)tileDimensionsNode->IntAttribute( "height" );
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
    if( ( image->w % tileset->tileWidth ) || ( image->h % tileset->tileHeight ) ){
        throw std::runtime_error( "ERROR: Image's' dimensions are not multiples of tile's dimensions" );
    }

    // Read tileset general info.
    tileset->imageWidth = image->w;
    tileset->imageHeight = image->h;
    tileset->nRows = ( image->h / tileset->tileHeight );
    tileset->nColumns = ( image->w / tileset->tileWidth );
    tileset->nTiles = tileset->nRows * tileset->nColumns;

    // Insert the tileset vertex attributes in the tilesetsBuffer and get its index.
    tileset->bufferIndex = tileset->tilesetsBuffer->insertTileset( tileset->tileWidth, tileset->tileHeight );

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

    if( developerMode ){
        // Set the texture's storage.
        glTexStorage3D( GL_TEXTURE_2D_ARRAY,    // target
                        1,                      // levels (1 = no mipmapping).
                        GL_RGBA8,               // internal format (32-bit textures).
                        tileset->imageWidth,     // texture width.
                        tileset->imageHeight,    // texture height.
                        1        // texture depth (number of slices).
                        );
        std::cout << "glTexStorage3D: " << gluErrorString( glGetError() ) << std::endl;

        // Set texture's image data.
        glTexSubImage3D( GL_TEXTURE_2D_ARRAY,   // target
                         0,                     // level
                         0,                     // xoffset
                         0,                     // yoffset
                         0,                  // zoffset
                         tileset->imageWidth,    // width
                         tileset->imageHeight,   // height
                         1,                     // depth
                         GL_RGBA,               // format
                         GL_UNSIGNED_BYTE,      // type
                         image->pixels          // image data.
                         );
    }else{
        // Set the texture's storage.
        glTexStorage3D( GL_TEXTURE_2D_ARRAY,    // target
                        1,                      // levels (1 = no mipmapping).
                        GL_RGBA8,               // internal format (32-bit textures).
                        tileset->tileWidth,     // texture width.
                        tileset->tileHeight,    // texture height.
                        tileset->nTiles         // texture depth (number of slices).
                        );
        std::cout << "glTexStorage3D: " << gluErrorString( glGetError() ) << std::endl;

        // Set texture's image data.
        GLuint tile = 0;
        for( GLuint row = 0; row < tileset->nRows; row++ ){
            for( GLuint column = 0; column < tileset->nColumns; column++ ){
                glPixelStorei( GL_UNPACK_SKIP_PIXELS, column*tileset->tileWidth );
                glPixelStorei( GL_UNPACK_SKIP_ROWS, row*tileset->tileHeight );
                glPixelStorei( GL_UNPACK_ROW_LENGTH, image->w );

                glTexSubImage3D( GL_TEXTURE_2D_ARRAY,   // target
                                 0,                     // level
                                 0,                     // xoffset
                                 0,                     // yoffset
                                 tile,                  // zoffset
                                 tileset->tileWidth,    // width
                                 tileset->tileHeight,   // height
                                 1,                     // depth
                                 GL_RGBA,               // format
                                 GL_UNSIGNED_BYTE,      // type
                                 image->pixels          // image data.
                                 );
                tile++;
            }

        }
    }

    std::cout << "glTextSubImage3D: " << gluErrorString( glGetError() ) << std::endl;
    glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );

    // Free the image's surface.
    SDL_FreeSurface( image );

    // Create an empty vector of collision rects for each tile in the tileset.
    tileset->collisionRects.resize( tileset->nTiles );

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
                for( unsigned int i=0; i<tileset->nTiles; i++ ){
                    tileset->collisionRects[i].push_back( colRect );
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
                    tileset->collisionRects[i].push_back( colRect );
                }
            }

            // Get the next collision rect XML node.
            collisionRectNode = collisionRectNode->NextSiblingElement( "collision_rect" );
        }
    }

    return tileset;
}


} // namespace m2g
