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

#include "bitmap_font.hpp"

namespace m2g {

// VBO_SIZE = 8 vertices (4 for world, 4 for texture) * 2D coordinates * number
// of ASCII printable characters.
const unsigned int VBO_ELEMENTS = 8 * 2 * N_ASCII_PRINTABLE_CHARACTERS;
const unsigned int VBO_SIZE = VBO_ELEMENTS * sizeof( GLfloat );

/***
 * 1. Initialization and destruction
 ***/

BitmapFont::BitmapFont() :
    vao( 0 ),
    vbo( 0 ),
    texture( 0 )
{
    // Generate a VAO name and bind it as the active one.
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Generate a VBO name and bind it as the active one.
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    // Allocate memory for the VBO.
    glBufferData( GL_ARRAY_BUFFER, VBO_SIZE, NULL, GL_STATIC_DRAW );

    // Set vertex attribute pointers.

    // We are sending 2D vertices to the vertex shader.
    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*)(0) );
    glEnableVertexAttribArray( 0 );

    // We are sending 2D texture coordinates to the shader.
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*)(2*sizeof(GLfloat)) );
    glEnableVertexAttribArray( 1 );

    checkOpenGL( "BitmapFont constructor" );
}


BitmapFont::~BitmapFont()
{
    // Free OpenGL resources.
    glDeleteVertexArrays( 1, &vao );
    glDeleteBuffers( 1, &vbo );
    glDeleteTextures( 1, &texture );
}


/***
 * 2. Loading
 ***/

void BitmapFont::load( const char* fontPath, unsigned int size )
{
    TTF_Font* font;

    char printableCharacters[N_ASCII_PRINTABLE_CHARACTERS+1];
    unsigned int i = 0, j;
    int imageWidth, imageHeight;
    SDL_Color fontColor = { 255, 0, 0, 255 };
    int letterHeight;
    GLfloat accumWidth = 0.0f;

    GLfloat vertexData[VBO_ELEMENTS];

    // Load the font
    font = TTF_OpenFont( fontPath, size );

    if( font == nullptr ){
        throw std::runtime_error( std::string( "ERROR opening font - " ) + TTF_GetError() );
    }

    // Set the RGBA mask for the text surface.
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        const Uint32 rmask = 0xff000000;
        const Uint32 gmask = 0x00ff0000;
        const Uint32 bmask = 0x0000ff00;
        const Uint32 amask = 0x000000ff;
    #else
        const Uint32 rmask = 0x000000ff;
        const Uint32 gmask = 0x0000ff00;
        const Uint32 bmask = 0x00ff0000;
        const Uint32 amask = 0xff000000;
    #endif

    // First ASCII printable character.
    char character[2] = { ' ', '\0' };
    SDL_Surface* auxTextSurface = nullptr;
    SDL_Surface* textSurface = nullptr;

    // Insert all the printable characters in one string.
    for( ; i<N_ASCII_PRINTABLE_CHARACTERS; i++ ){
        printableCharacters[i] = i + ' ';
        //std::cout << "[" << printableCharacters[i] << "] : " <<  TTF_GlyphIsProvided( font, (short int)( printableCharacters[i] ) ) << std::endl;
    }
    printableCharacters[i] = 0;

    std::cout << "Printable characters: [" << printableCharacters << "]" << std::endl;

    // Render all the printable characters in one surface for getting
    // the full dimensions.
    auxTextSurface = TTF_RenderText_Blended( font, printableCharacters, fontColor );
    std::cout << "auxTextSurface dimensions: " << auxTextSurface->w << " x " << auxTextSurface->h << std::endl;

    // Round the image width to the nearest pow of 2.
    imageWidth = 1;
    while( imageWidth < auxTextSurface->w ){
        imageWidth <<= 1;
    }

    // Round the image height to the nearest pow of 2.
    imageHeight = 1;
    while( imageHeight < auxTextSurface->h ){
        imageHeight <<= 1;
    }

    // Create the final text surface with the power-of-two dimensions.
    textSurface = SDL_CreateRGBSurface( 0, imageWidth, imageHeight, 32, rmask, gmask, bmask, amask );
    std::cout << "Image dimensions: " << imageWidth << " x " << imageHeight << std::endl;

    // Copy all the printable characters to the final power-of-two surface.
    SDL_BlitSurface( auxTextSurface, NULL, textSurface, NULL );

    // Fill the BitmapFont VBO with the vertex attributes of each printable
    // character.
    for( i = 0; i < N_ASCII_PRINTABLE_CHARACTERS; i++ ){

        // Get the dimensions of the current character.
        if( TTF_SizeText( font, character, &widths[i], &letterHeight ) ){
            throw std::runtime_error( std::string( "ERROR in TTF_SizeText() - " ) + TTF_GetError() );
        }

        j = i * 16;

        // Set the vertex attributes for the current character.

        /*** Bottom left ***/
        // Vertice coordinates
        vertexData[j] = 0.0f;
        vertexData[j+1] = 0.0f;
        // Texture coordinates
        vertexData[j+2] = accumWidth / (GLfloat)imageWidth;
        vertexData[j+3] = 0.0f;

        /*** Bottom right ***/
        // Vertice coordinates
        vertexData[j+4] = widths[i];
        vertexData[j+5] = 0.0f;

        // Texture coordinates
        vertexData[j+6] = (accumWidth + widths[i]) / (GLfloat)imageWidth;
        vertexData[j+7] = 0.0f;

        /*** Top left ***/
        // Vertice coordinates
        vertexData[j+8] = 0.0f;
        vertexData[j+9] = letterHeight;

        // Texture coordinates
        vertexData[j+10] = accumWidth / (GLfloat)imageWidth;
        vertexData[j+11] = letterHeight / (GLfloat)imageHeight;

        /*** Top right ***/
        // Vertice coordinates
        vertexData[j+12] = widths[i];
        vertexData[j+13] = letterHeight;

        // Texture coordinates
        vertexData[j+14] = (accumWidth + widths[i]) / (GLfloat)imageWidth;
        vertexData[j+15] = letterHeight / (GLfloat)imageHeight;

        // Iterate to the following character.
        character[0]++;
        accumWidth += widths[i];
    }

    checkOpenGL( "BitmapConstructor (1)" );
    SDL_SaveBMP( textSurface, "bmp" );

    // Copy all the vertexData to the VBO
    glBufferSubData( GL_ARRAY_BUFFER, 0, VBO_SIZE, vertexData );

    checkOpenGL( "BitmapConstructor (2)" );

    /*** Load the texture ***/
    // Generate the texture and set its parameters.
    // TODO: play with multiple texture units (or not?).
    glActiveTexture( GL_TEXTURE0 );
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D_ARRAY, texture );

    glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
    glPixelStorei( GL_UNPACK_SKIP_PIXELS, 0 );
    glPixelStorei( GL_UNPACK_SKIP_ROWS, 0 );
    glPixelStorei( GL_UNPACK_ROW_LENGTH, textSurface->w );

    //glPixelStorei( GL_UNPACK_ROW_LENGTH,  );

    checkOpenGL( "BitmapConstructor (3)" );


    // Set the texture data.
    SDL_LockSurface( textSurface );
    glTexImage3D( GL_TEXTURE_2D_ARRAY,     // target
                  0,                       // Level
                  GL_RGBA8,                // internal format (32-bit textures).
                  imageWidth,              // Image width
                  imageHeight,             // Image height
                  1,                       // Depth
                  0,                       // Border (must be 0).
                  GL_RGBA,                 // Format
                  GL_UNSIGNED_BYTE,        // type
                  textSurface->pixels      // image data.
                );
    SDL_UnlockSurface( textSurface );

    checkOpenGL( "BitmapConstructor (5)" );

    // glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );

    // Free the allocated resources.
    SDL_FreeSurface( auxTextSurface );
    SDL_FreeSurface( textSurface );
    TTF_CloseFont( font );
}


/***
 * 3. Getters
 ***/

unsigned int BitmapFont::getCharacterWidth( char c ) const
{
    return widths[ c - ' ' ];
}


/***
 * 4. Drawing
 ***/

void BitmapFont::bind() const
{
    // Bind the buffer's VAO and VBO as the active ones.
    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D_ARRAY, texture );
}


void BitmapFont::drawCharacter( char c ) const
{
    int index = c - ' ';

    // Draw the requested rect.
    glDrawArrays( GL_TRIANGLE_STRIP, index*4, 4 );

    //std::cout << "Drawing character (" << c << ") [index: " << index << "]" << std::endl;
}


} // namespace m2g
