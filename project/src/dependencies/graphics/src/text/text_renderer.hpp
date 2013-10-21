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

#ifndef TEXT_RENDERER_HPP
#define TEXT_RENDERER_HPP

#include "bitmap_font.hpp"
#include <SDL2/SDL_ttf.h>

namespace m2g {

class TextRenderer
{
    private:
        std::vector< BitmapFontPtr > bitmapFonts;

        GLint mvpMatrixLocation;
        GLint samplerLocation;
        GLint sliceLocation;

    public:
        /***
         * 1. Initialization
         ***/
        TextRenderer();


        /***
         * 2. Loading
         ***/
        unsigned int loadFont( const char* file, const unsigned int size, const SDL_Color& color );


        /***
         * 3. Drawing
         ***/
        void drawText( const glm::mat4& projectionMatrix, const char* text, unsigned int fontIndex, GLuint x = 0, GLuint y = 0 );
};

} // namespace m2g

#endif // TEXT_RENDERER_HPP
