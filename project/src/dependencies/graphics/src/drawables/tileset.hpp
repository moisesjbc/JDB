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

#ifndef TILESET_HPP
#define TILESET_HPP

#include "drawable.hpp"
#include <memory>
#include <iostream>
#include <vector>
#include "../utilities/tilesets_buffer.hpp"
#include "../dependencies/tinyxml2/tinyxml2.h"
#include <SDL2/SDL_ttf.h>

namespace m2g {

class Tileset {

    public:
        // Common tilesets buffer for all tilesets and its corresponding references
        // count.
        static TilesetsBuffer* tilesetsBuffer;
        static unsigned int refCount;

        // Texture 2D array id (OpenGL).
        GLuint texture;

        // Tile dimensions.
        GLuint tileWidth;
        GLuint tileHeight;

        // Image dimensions.
        GLuint imageWidth;
        GLuint imageHeight;

        // Tileset number of elements.
        GLuint nRows;
        GLuint nColumns;
        GLuint nTiles;

        // Index of this tileset's vertex attributes in the tilesets buffer.
        unsigned int bufferIndex;

        // Name of the tileset's base image.
        std::string name;

        // We keep a vector of collision rects for each tile in the tileset.
        std::vector< std::vector< Rect > > collisionRects;


        /*** Methods ***/

        /***
         * 1. Initialization and destruction.
         ***/
        Tileset( const tinyxml2::XMLNode* xmlNode, const char* folder );
        Tileset( SDL_Surface* surface, GLuint tileWidth, GLuint tileHeight );
        Tileset( TTF_Font* font, unsigned int size );
        ~Tileset();


        /***
         * 2. Loading
         ***/
        void load( const tinyxml2::XMLNode* xmlNode, const char* folder );
        void load( SDL_Surface* surface, GLuint tileWidth, GLuint tileHeight );
        void load( TTF_Font* font, unsigned int size );


        /***
         * 3. Drawing
         ***/
        void draw() const ;


        /***
         * 4. Auxiliar methods
         ***/
        static void bindBuffer();
    private:
        void loadTexture( void* data, int pitch  );
};

typedef std::shared_ptr< const Tileset > TilesetPtr;

} // Namespace m2g

#endif // TILESET_HPP
