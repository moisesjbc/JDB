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

#ifndef GRAPHICS_LIBRARY_HPP
#define GRAPHICS_LIBRARY_HPP

#include "drawables/sprite.hpp"
#include "drawables/animation_data.hpp"
#include "dependencies/tinyxml2/tinyxml2.h"
#include "drawables/animation.hpp"
#include <string>

namespace m2g {

typedef std::vector< std::shared_ptr<Tileset> > TilesetsVector;
typedef std::vector< std::shared_ptr<AnimationData> > AnimationDataVector;

class GraphicsLibrary
{
    private:
        tinyxml2::XMLDocument xmlFile_;

        TilesetsVector tilesets_;
        AnimationDataVector animationData_;

    public:
        /***
         * 1. Initialization and destruction.
         ***/
        GraphicsLibrary();


        /***
         * 2. Loading methods
         ***/
        void loadAll( std::string libraryFolder );


        /***
         * 3. Getters
         ***/
        TilesetPtr getTileset( std::string name ) const ;

        AnimationDataPtr getAnimationData( std::string name ) const ;
        AnimationDataVector getAnimationDataByPrefix ( std::string prefix ) const ;


        /*
        void loadTilesets( TilesetsVector& tilesets, std::string libraryFolder );
        void loadTilesets( TilesetsVector& tilesets, std::string libraryFolder, std::string prefix );
        void loadTileset( TilesetPtr& tileset, std::string libraryFolder, std::string name );

        void loadAnimationsData( AnimationDataVector& animationData, std::string libraryFolder );
        void loadAnimationsData( AnimationDataVector& animationData, std::string libraryFolder, std::string prefix );
        void loadAnimationData( AnimationDataPtr& animationData, std::string libraryFolder, std::string name );
        */

        /***
         * 4. Auxiliar methods
         ***/
        void clear();
    private:
        void getLibraryPaths( std::string libraryFolder, std::string& imagesFolder, std::string& libraryFile );
        tinyxml2::XMLNode* getTilesetsRootNode( std::string libraryFile );
        tinyxml2::XMLNode* getAnimationDataRootNode( std::string libraryFile );
};

} // namespace m2g

#endif // GRAPHICS_LIBRARY_HPP
