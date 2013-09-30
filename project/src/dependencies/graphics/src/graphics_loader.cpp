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

#include "graphics_loader.hpp"

namespace m2g {

/***
 * 1. Initialization and destruction.
 ***/

GraphicsLoader::GraphicsLoader()
{}


/***
 * 2. Loading methods
 ***/

void GraphicsLoader::loadTilesets( TilesetsVector& tilesets, std::string libraryFolder )
{
    tinyxml2::XMLNode* tilesetNode = nullptr;
    std::string imagesFolder;
    std::string libraryFile;

    // Use the library folder path to get the paths to the images folder and metadata file.
    getLibraryPaths( libraryFolder, imagesFolder, libraryFile );

    // Load the "tilesets" XML node.
    tilesetNode = getTilesetsRootNode( libraryFile );

    if( tilesetNode ){
        // Get the first XML tileset node.
        tilesetNode = tilesetNode->FirstChildElement();

        // Keep reading XML tileset nodes.
        while( tilesetNode ){
            // Load the tileset from the current XML element.
            tilesets.push_back( std::shared_ptr< Tileset >( new Tileset( tilesetNode, imagesFolder.c_str() ) ) );

            // Load next XML element.
            tilesetNode = tilesetNode->NextSiblingElement();
        }
    }
}


void GraphicsLoader::loadTilesets( TilesetsVector& tilesets, std::string libraryFolder, std::string prefix )
{
    tinyxml2::XMLNode* tilesetNode = nullptr;
    std::string imagesFolder;
    std::string libraryFile;
    std::string currentName;

    // Use the library folder path to get the paths to the images folder and metadata file.
    getLibraryPaths( libraryFolder, imagesFolder, libraryFile );

    // Load the "tilesets" XML node.
    tilesetNode = getTilesetsRootNode( libraryFile );

    if( tilesetNode ){
        // Get the first XML tileset node.
        tilesetNode = tilesetNode->FirstChildElement();

        // Keep reading XML tileset nodes.
        while( tilesetNode ){
            currentName = std::string( tilesetNode->FirstChildElement( "src" )->GetText() );

            if( currentName.compare( 0, prefix.size(), prefix ) == 0  ){
                // If the current tileset's name starts with prefix, add it to the
                // tilesets vector.
                tilesets.push_back( std::shared_ptr< Tileset >( new Tileset( tilesetNode, imagesFolder.c_str() ) ) );
            }

            // Load next XML element.
            tilesetNode = tilesetNode->NextSiblingElement();
        }
    }
}


void GraphicsLoader::loadAnimationsData( AnimationDataVector& animationsData, std::string libraryFolder )
{
    tinyxml2::XMLNode* animationDataNode = nullptr;
    std::string imagesFolder;
    std::string libraryFile;

    // Use the library folder path to get the paths to the images folder and metadata file.
    getLibraryPaths( libraryFolder, imagesFolder, libraryFile );

    // Load the "animations" XML node.
    animationDataNode = getAnimationDataRootNode( libraryFile );

    if( animationDataNode ){
        // Get the first XML tileset node.
        animationDataNode = animationDataNode->FirstChildElement();

        // Keep reading XML tileset nodes.
        while( animationDataNode ){
            // Load the tileset from the current XML element.
            animationsData.push_back( std::shared_ptr< AnimationData >( new AnimationData( animationDataNode, imagesFolder.c_str() ) ) );

            // Load next XML element.
            animationDataNode = animationDataNode->NextSiblingElement();
        }
    }
}


void GraphicsLoader::loadAnimationsData( AnimationDataVector& animationsData, std::string libraryFolder, std::string prefix )
{
    tinyxml2::XMLNode* animationDataNode = nullptr;
    std::string imagesFolder;
    std::string libraryFile;
    std::string currentName;

    // Use the library folder path to get the paths to the images folder and metadata file.
    getLibraryPaths( libraryFolder, imagesFolder, libraryFile );

    // Load the "animations" XML node.
    animationDataNode = getAnimationDataRootNode( libraryFile );

    if( animationDataNode ){
        // Get the first XML animationData node.
        animationDataNode = animationDataNode->FirstChildElement();

        // Keep reading XML animationData nodes.
        while( animationDataNode ){
            currentName = std::string( animationDataNode->FirstChildElement( "tileset" )->FirstChildElement( "src" )->GetText() );

            if( currentName.compare( 0, prefix.size(), prefix ) == 0  ){
                // If the current animationData's name starts with prefix, add it to the
                // animationsData vector.
                animationsData.push_back( std::shared_ptr< AnimationData >( new AnimationData( animationDataNode, imagesFolder.c_str() ) ) );
            }

            // Load next XML element.
            animationDataNode = animationDataNode->NextSiblingElement();
        }
    }
}

/***
 * 3. Auxiliar methods
 ***/

void GraphicsLoader::getLibraryPaths( std::string libraryFolder, std::string& imagesFolder, std::string& libraryFile )
{
    // The library folder's path can contain a '/' or not at the end, so
    // the path is "normarlized" here by removing that last '/', if it exists.
    if( libraryFolder[libraryFolder.size()-1] == '/' ){
        libraryFolder.resize( libraryFolder.size () - 1 );
    }

    // Get the path to the "default" images folder.
    // TODO: Allow other skins loading.
    imagesFolder = libraryFolder + "/default/";

    // Get the path to the metadata file.
    libraryFile = libraryFolder + "/graphic_library.xml";
}


tinyxml2::XMLNode* GraphicsLoader::getTilesetsRootNode( std::string libraryFile )
{
    // Open the requested file.
    xmlFile.LoadFile( libraryFile.c_str() );

    // Return the tilesets root node.
    return ( xmlFile.FirstChildElement( "library" ) )->FirstChildElement( "tilesets" );
}


tinyxml2::XMLNode* GraphicsLoader::getAnimationDataRootNode( std::string libraryFile )
{
    // Open the requested file.
    xmlFile.LoadFile( libraryFile.c_str() );

    // Return the tilesets root node.
    return ( xmlFile.FirstChildElement( "library" ) )->FirstChildElement( "animations" );
}

} // namespace m2g
