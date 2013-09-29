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

#ifndef LIBRARY_HPP
#define LIBRARY_HPP

#include "drawables/sprite.hpp"
#include "drawables/animation_data.hpp"
#include "dependencies/tinyxml2/tinyxml2.h"
#include "drawables/animation.hpp"
#include <string>

namespace m2g {

typedef std::vector< std::shared_ptr<Tileset> > TilesetsVector;
typedef std::vector< std::shared_ptr<Animation> > AnimationDataVector;

class Library
{
    private:
        tinyxml2::XMLDocument xmlFile;

    public:
        /***
         * 1. Initialization and destruction.
         ***/
        Library();


        /***
         * 2. Loading methods
         ***/
        void loadTilesets( TilesetsVector& tilesets, std::string libraryFolder );
        //AnimationDataVector loadAnimationData( string libraryFolder );


        /***
         * 3. Auxiliar methods
         ***/
    private:
        void getLibraryPaths( std::string libraryFolder, std::string& imagesFolder, std::string& libraryFile );
        tinyxml2::XMLNode* getTilesetsRootNode( std::string libraryFile );
        //tinyxml2::XMLNode* getAnimationDataRootNode( std::string libraryFile );


        /***
         * 2. File management
         ***
        void load( std::string libraryFolder );


        /***
         * 3. Setters and getters
         ***
        const std::shared_ptr< m2g::Tileset > getTileset( const unsigned int& index ) const ;
        const std::shared_ptr< m2g::Tileset> getTileset( const std::string& imageName ) const ;

        const std::shared_ptr< m2g::AnimationData > getAnimationData( const unsigned int& index ) const ;
        const std::shared_ptr< m2g::AnimationData> getAnimationData( const std::string& imageName ) const ;


        /***
         * 4. Tileset and animation data loading
         ***
    private:
        std::shared_ptr<Tileset> loadTileset( const tinyxml2::XMLNode* xmlNode, const char* folder = nullptr );
        ***/
};

} // namespace m2g

#endif // LIBRARY_HPP
