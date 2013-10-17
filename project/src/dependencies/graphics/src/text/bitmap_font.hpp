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

#ifndef BITMAP_FONT_HPP
#define BITMAP_FONT_HPP

#include "../utilities/tilesets_buffer.hpp"
#include <memory>
#include <SDL2/SDL_ttf.h>

namespace m2g {

const unsigned int N_ASCII_PRINTABLE_CHARACTERS = 95;

class BitmapFont
{
    private:
        // VAO and VBO.
        GLuint vao;
        GLuint vbo;

        // Texture 2D array id (OpenGL).
        GLuint texture;


        int widths[N_ASCII_PRINTABLE_CHARACTERS+1];

        int offset;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        BitmapFont();
        ~BitmapFont();


        /***
         * 2. Loading
         ***/
        void load( const char* fontPath, unsigned int size );
    private:
        void loadTexture( void* data, int pitch  );
    public:


        /***
         * 3. Getters
         ***/
        unsigned int getCharacterWidth( char c ) const ;

        /***
         * 4. Drawing
         ***/
        void bind() const ;
        void drawCharacter( char c ) const ;
};


typedef std::shared_ptr< const BitmapFont > BitmapFontPtr;

} // namespace m2g

#endif // BITMAP_FONT_HPP
