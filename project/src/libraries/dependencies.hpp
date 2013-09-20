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

#ifndef DEPENDENCIES_HPP
#define DEPENDENCIES_HPP

#define GL_GLEXT_PROTOTYPES

// GL3W
#include "gl3w/gl3w.h"

// OpenGL
//#include <GL/glext.h>
#include <GL/gl.h>
//#include <GL/glx.h>
#include <GL/glu.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/gl.h>

// SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>

// Others
#include <stdexcept>

#endif // DEPENDENCIES_HPP
