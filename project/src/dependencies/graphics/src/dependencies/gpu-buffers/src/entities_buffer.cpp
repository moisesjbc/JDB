/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of GPU Buffers.
 *
 * GPU Buffers is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * GPU Buffers is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GPU Buffers.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "entities_buffer.hpp"

namespace gpu_buf {

/***
 * 1. Initialization and destruction
 ***/

EntitiesBuffer::EntitiesBuffer( GLenum target_, GLenum usage_, GLuint totalSize_, GLuint verticesPerEntity_ ) :
    Buffer( target_, usage_, totalSize_ ),
    VERTICES_PER_ENTITY( verticesPerEntity_ )
{
    std::cout << "Entities constructor - " << gluErrorString( glGetError() ) << std::endl;
}

} // namespace gpu_buf
