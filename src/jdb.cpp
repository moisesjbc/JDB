/***
    Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)

    This file is part of JDB.

    JDB is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    JDB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with JDB.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#include "jdb.hpp"

namespace jdb {

JDB::JDB() :
    window( NULL ),
    screen( NULL )
{
    // Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        throw std::runtime_error( SDL_GetError() );
    }

    // Create main window
    window = SDL_CreateWindow(
      "JDB v3",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      800,
      600,
      SDL_WINDOW_SHOWN );

    screen = SDL_GetWindowSurface( window );
}


JDB::~JDB()
{
    // Destroy the main window (muahaha).
    SDL_DestroyWindow( window );
    window = NULL;

    // Destroy SDL (muahaha x2).
    SDL_Quit();
}


void JDB::run()
{
    SDL_Event event;
    bool quit = false;

    // Keep rendering a black window until player tell us to stop.
    while( !quit ){
       while( SDL_PollEvent( &event ) != 0 ){
          if( event.type == SDL_QUIT ){
             quit = true;
          }
       }
       //SDL_BlitSurface( image, NULL, screen, NULL );
       SDL_UpdateWindowSurface( window );
    }
}

} // namespace jdb
