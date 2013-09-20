/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of Graphic Librarian.
 *
 * Graphic Librarian is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Graphic Librarian is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Graphic Librarian.  If not, see <http://www.gnu.org/licenses/>.
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
    GLuint texture;

    GLuint tileWidth;
    GLuint tileHeight;

    GLuint imageWidth;
    GLuint imageHeight;

    GLuint nRows;
    GLuint nColumns;
    GLuint nTiles;

    GLuint vbo;

    char name[MAX_TILESET_NAME_SIZE];

    std::vector< Rect > generalCollisionRects;
    //std::vector< std::vector <Rect> > collisionRects;

    Tileset();
    ~Tileset();
};

} // Namespace m2g

#endif // TILESET_HPP
