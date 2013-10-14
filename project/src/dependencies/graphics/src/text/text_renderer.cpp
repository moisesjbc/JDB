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

#include "text_renderer.hpp"

namespace m2g {


/***
 * 2. Loading
 ***/

unsigned int TextRenderer::loadFont( const char* file, const unsigned int size )
{
    m2g::TilesetPtr textTileset;

    // Load the font
    TTF_Font* font = TTF_OpenFont( file, size );

    if( !font ){
        throw std::runtime_error( std::string( "ERROR opening font - " ) + TTF_GetError() );
    }

    // Generate the tileset (bitmap font) from the TrueType font.
    textTileset = std::make_shared<m2g::Tileset>( font, 16 );

    // Insert the new bitmap font in the bitmap fonts vector.
    bitmapFonts.push_back( textTileset );

    // Return the index of the just added font.
    return bitmapFonts.size() - 1;
}


/***
 * 3. Drawing
 ***/

void TextRenderer::drawText( const glm::mat4& projectionMatrix, const char* text, unsigned int fontIndex, GLuint x, GLuint y )
{
    unsigned int i = 0;

    sprite.setTileset( bitmapFonts[ fontIndex ] );

    sprite.moveTo( x, y );

    for( ; i < strlen( text ); i++ ){
        sprite.setTile( text[i] - ' ' );
        sprite.draw( projectionMatrix );

        std::cout << "Translating: " << bitmapFonts[fontIndex]->tileWidth << std::endl;
        sprite.translate( bitmapFonts[fontIndex]->width[ text[i] - ' ' ], 0.0f );
    }
}

} // namespace m2g
