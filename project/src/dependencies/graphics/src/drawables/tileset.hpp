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

namespace m2g {

const unsigned int MAX_TILESET_NAME_SIZE = 100;

struct Tileset {
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

    // VBO with tileset vertices and UV coordinates.
    GLuint vbo;

    // Name of the tileset's base image.
    char name[MAX_TILESET_NAME_SIZE];

    // We keep a vector of collision rects for each tile in the tileset.
    std::vector< std::vector< Rect > > collisionRects;

    /***
     * 1. Initialization and destruction.
     ***/
    Tileset();
    ~Tileset();
};

} // Namespace m2g

#endif // TILESET_HPP
