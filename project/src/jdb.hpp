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

#include "dependencies/dependencies.hpp"
#include "dependencies/msl/src/shader_loader.hpp"
#include "dependencies/graphics/src/graphics_loader.hpp"

namespace jdb {

class JDB
{
    private:
        GLuint vao;
        SDL_Window* window;
        SDL_Surface* screen;
        SDL_GLContext glContext;
        msl::ShaderLoader* shaderLoader;
        glm::mat4 projectionMatrix;

    public:
        JDB();
        ~JDB();

        void runInfiniteSandwichesDemo();
};

} // namespace jdb

#endif // JDB_HPP
