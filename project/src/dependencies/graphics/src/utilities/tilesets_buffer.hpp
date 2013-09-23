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

#ifndef TILESETS_BUFFER_HPP
#define TILESETS_BUFFER_HPP

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include <GL/glu.h>
#include <iostream>

namespace m2g {

class TilesetsBuffer
{
    private:
        // Buffer's associated VAO and VBO.
        GLuint vao;
        GLuint vbo;

        // Buffer's occupied and total sizes (in bytes).
        GLuint occupiedSize;
        GLuint totalSize;

        GLuint nTilesets;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        TilesetsBuffer( GLuint nTilesets_ );
        ~TilesetsBuffer();


        /***
         * 2. Setters and getters
         ***/


        /***
         * 3. Data insertion.
         ***/
        void insertTileset( GLfloat tileWidth, GLfloat tileHeight );


        /***
         * 4. Drawing.
         ***/
        virtual void draw( GLuint index );


        /***
         * 5. Auxiliar methods
         ***/
        virtual void clear();
};

} // namespace m2g

#endif // TILESETS_BUFFER_HPP
