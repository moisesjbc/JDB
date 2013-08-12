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

#ifndef JDB_HPP
#define JDB_HPP

#include "graphics/tileset.hpp"
#include "utilities/shader_loader.hpp"

namespace jdb {

class JDB
{
    private:
        SDL_Window* window;
        SDL_Surface* screen;
        SDL_GLContext glContext;
        ShaderLoader* shaderLoader;

    public:
        JDB();
        ~JDB();

        void run();
};

} // namespace jdb

#endif // JDB_HPP
