/***
    Copyright 2013 - 2015 Moises J. Bonilla Caraballo (Neodivert)

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

#include "window.hpp"
#include <stdexcept>

namespace jdb {

/***
 * 1. Construction
 ***/

Window::Window( SDL_Window* window ) :
    window( window )
{
    renderer = SDL_CreateRenderer( window, -1, 0 );
    if( !renderer ){
        throw std::runtime_error( SDL_GetError() );
    }
}


/***
 * 2. Getters
 ***/

int Window::width() const
{
    int width = 0;
    SDL_GetWindowSize( window, &width, nullptr );
    return width;
}


int Window::height() const
{
    int height = 0;
    SDL_GetWindowSize( window, nullptr, &height );
    return height;
}

} // namespace jdb
