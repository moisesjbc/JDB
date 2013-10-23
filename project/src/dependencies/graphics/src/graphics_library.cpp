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

#include "graphics_library.hpp"

namespace m2g {

/***
 * 1. Initialization and destruction.
 ***/

GraphicsLibrary::GraphicsLibrary()
{}


/***
 * 2. Loading methods
 ***/

void GraphicsLibrary::loadAll( std::string libraryFolder )
{
    tinyxml2::XMLNode* xmlNode = nullptr;
    std::string imagesFolder;
    std::string libraryFile;

    std::cout << "Loading [" << libraryFolder.c_str() << "] (1)" << std::endl;

    // Use the library folder path to get the paths to the images folder and metadata file.
    getLibraryPaths( libraryFolder, imagesFolder, libraryFile );

    std::cout << "Loading [" << libraryFolder.c_str() << "] (2)" << std::endl;
    // Load the "tilesets" XML node.
    xmlNode = getTilesetsRootNode( libraryFile );

    if( xmlNode ){
        // Get the first XML tileset node.
        xmlNode = xmlNode->FirstChildElement();

        // Keep reading XML tileset nodes.
        while( xmlNode ){
            // Load the tileset from the current XML element.
            tilesets_.push_back( std::shared_ptr< Tileset >( new Tileset( xmlNode, imagesFolder.c_str() ) ) );

            // Load next XML element.
            xmlNode = xmlNode->NextSiblingElement();
        }
    }

    std::cout << "Loading [" << libraryFolder.c_str() << "] (3)" << std::endl;
    // Load the "animations" XML node.
    xmlNode = getAnimationDataRootNode( libraryFile );

    if( xmlNode ){
        // Get the first XML animationData node.
        xmlNode = xmlNode->FirstChildElement();

        // Keep reading XML tileset nodes.
        while( xmlNode ){
            // Load the tileset from the current XML element.
            animationData_.push_back( std::shared_ptr< AnimationData >( new AnimationData( xmlNode, imagesFolder.c_str() ) ) );

            // Load next XML element.
            xmlNode = xmlNode->NextSiblingElement();
        }
    }
    std::cout << "Loading [" << libraryFolder.c_str() << "] (4)" << std::endl;
}


/***
 * 3. Getters
 ***/

TilesetPtr GraphicsLibrary::getTileset( std::string name ) const
{
    unsigned int i = 0;
    TilesetPtr nullPtr;

    for( ; i<tilesets_.size(); i++ ){
        if( tilesets_[i]->name == name ){
            return tilesets_[i];
        }
    }

    return nullPtr;
}


AnimationDataPtr GraphicsLibrary::getAnimationData( std::string name ) const
{
    unsigned int i = 0;
    AnimationDataPtr nullPtr;

    for( ; i<animationData_.size(); i++ ){
        if( animationData_[i]->tileset->name == name ){
            return animationData_[i];
        }
    }

    return nullPtr;
}


AnimationDataVector GraphicsLibrary::getAnimationDataByPrefix ( std::string prefix ) const
{
    unsigned int i = 0;
    AnimationDataVector animationData;

    for( ; i<animationData_.size(); i++ ){
        if( animationData_[i]->tileset->name.compare( 0, prefix.size(), prefix ) == 0  ){
            animationData.push_back( animationData_[i] );
        }
    }

    return animationData;
}


/*
void GraphicsLibrary::loadTilesets( TilesetsVector& tilesets, std::string libraryFolder )
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


void GraphicsLibrary::loadTilesets( TilesetsVector& tilesets, std::string libraryFolder, std::string prefix )
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


void GraphicsLibrary::loadTileset( TilesetPtr& tileset, std::string libraryFolder, std::string name )
{
    tinyxml2::XMLNode* tilesetNode = nullptr;
    std::string imagesFolder;
    std::string libraryFile;
    std::string currentName;
    bool found = false;

    // Use the library folder path to get the paths to the images folder and metadata file.
    getLibraryPaths( libraryFolder, imagesFolder, libraryFile );

    // Load the "tilesets" XML node.
    tilesetNode = getTilesetsRootNode( libraryFile );

    if( tilesetNode ){
        // Get the first XML tileset node.
        tilesetNode = tilesetNode->FirstChildElement();

        // Keep reading XML tileset nodes.
        while( tilesetNode && !found ){
            currentName = std::string( tilesetNode->FirstChildElement( "src" )->GetText() );

            if( currentName.compare( name ) == 0  ){
                tileset = TilesetPtr( new Tileset( tilesetNode, imagesFolder.c_str() ) );
                found = true;
            }

            // Load next XML element.
            tilesetNode = tilesetNode->NextSiblingElement();
        }
    }
}


void GraphicsLibrary::loadAnimationsData( AnimationDataVector& animationsData, std::string libraryFolder )
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


void GraphicsLibrary::loadAnimationsData( AnimationDataVector& animationsData, std::string libraryFolder, std::string prefix )
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

void GraphicsLibrary::loadAnimationData( AnimationDataPtr& animationData, std::string libraryFolder, std::string name )
{
    tinyxml2::XMLNode* animationDataNode = nullptr;
    std::string imagesFolder;
    std::string libraryFile;
    std::string currentName;
    bool found = false;

    // Use the library folder path to get the paths to the images folder and metadata file.
    getLibraryPaths( libraryFolder, imagesFolder, libraryFile );

    // Load the "animations" XML node.
    animationDataNode = getAnimationDataRootNode( libraryFile );

    if( animationDataNode ){
        // Get the first XML animationData node.
        animationDataNode = animationDataNode->FirstChildElement();

        // Keep reading XML animationData nodes.
        while( animationDataNode && !found ){
            currentName = std::string( animationDataNode->FirstChildElement( "tileset" )->FirstChildElement( "src" )->GetText() );

            if( currentName.compare( name ) == 0  ){
                animationData = std::shared_ptr< AnimationData >( new AnimationData( animationDataNode, imagesFolder.c_str() ) );
                found = true;
            }

            // Load next XML element.
            animationDataNode = animationDataNode->NextSiblingElement();
        }
    }
}
*/


/***
 * 4. Auxiliar methods
 ***/

void GraphicsLibrary::clear()
{
    tilesets_.clear();
    animationData_.clear();
}


void GraphicsLibrary::getLibraryPaths( std::string libraryFolder, std::string& imagesFolder, std::string& libraryFile )
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


tinyxml2::XMLNode* GraphicsLibrary::getTilesetsRootNode( std::string libraryFile )
{
    // Open the requested file.
    xmlFile_.LoadFile( libraryFile.c_str() );

    // Return the tilesets root node.
    return ( xmlFile_.FirstChildElement( "library" ) )->FirstChildElement( "tilesets" );
}


tinyxml2::XMLNode* GraphicsLibrary::getAnimationDataRootNode( std::string libraryFile )
{
    // Open the requested file.
    xmlFile_.LoadFile( libraryFile.c_str() );

    // Return the animationsData root node.
    return ( xmlFile_.FirstChildElement( "library" ) )->FirstChildElement( "animations" );
}

} // namespace m2g
