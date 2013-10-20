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
 * 1. Initialization
 ***/

TextRenderer::TextRenderer()
{
    GLint currentProgram;

    // Get the uniforms locations.
    glGetIntegerv( GL_CURRENT_PROGRAM, &currentProgram );

    mvpMatrixLocation = glGetUniformLocation( currentProgram, "mvpMatrix" );
    samplerLocation = glGetUniformLocation( currentProgram, "tex" );
    sliceLocation = glGetUniformLocation( currentProgram, "slice" );

    checkOpenGL( "TextRenderer - Getting uniform locations" );

    // Connect sampler to texture unit 0.
    glUniform1i( samplerLocation, 0 );
}

/***
 * 2. Loading
 ***/

unsigned int TextRenderer::loadFont( const char* file, const unsigned int size )
{
    std::shared_ptr< m2g::BitmapFont > bitmapFont = std::shared_ptr< m2g::BitmapFont >( new m2g::BitmapFont );

    // Generate the bitmap font from the TrueType one.
    bitmapFont->load( file, size );

    // Insert the new bitmap font in the bitmap fonts vector.
    bitmapFonts.push_back( bitmapFont );

    // Return the index of the just added font.
    return bitmapFonts.size() - 1;
}


/***
 * 3. Drawing
 ***/

void TextRenderer::drawText( const glm::mat4& projectionMatrix, const char* text, unsigned int fontIndex, GLuint x, GLuint y )
{
    unsigned int i = 0;
    glm::mat4 transformationMatrix;

    // Bind the bitmap font (its VAO, VBO and texture) as the active one.
    bitmapFonts[fontIndex]->bind();

    // Send current 0 index to shader.
    glUniform1ui( sliceLocation, 0 );

    for( ; i < strlen( text ); i++ ){
        // Set MVP matrix.
        transformationMatrix = projectionMatrix * glm::translate( glm::mat4( 1.0f ), glm::vec3( x, y, 0.0f ) );

        // Send the MVP matrix to the shader.
        glUniformMatrix4fv( mvpMatrixLocation, 1, GL_FALSE, &transformationMatrix[0][0] );

        // Draw the current character.
        glUniform1ui( sliceLocation, text[i]-' ' );
        bitmapFonts[fontIndex]->drawCharacter( text[i] );

        //std::cout << "Translating: " << bitmapFonts[fontIndex]->getCharacterWidth( text[i] - ' ' ) << std::endl;

        x += bitmapFonts[fontIndex]->getCharacterWidth( text[i] );

        //transformationMatrix = transformationMatrix * glm::translate( glm::mat4( 1.0f ), glm::vec3( bitmapFonts[fontIndex]->getCharacterWidth( text[i] - ' ' ) * 2, 0.0f, 0.0f ) );
    }
}

} // namespace m2g
