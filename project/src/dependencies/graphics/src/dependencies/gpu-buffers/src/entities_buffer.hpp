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

#ifndef ENTITIES_BUFFER_HPP
#define ENTITIES_BUFFER_HPP

#include "buffer.hpp"
#include <vector>

namespace gpu_buf {

class EntitiesBuffer : public Buffer
{
    protected:
        const GLuint VERTICES_PER_ENTITY;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        EntitiesBuffer( GLenum target_, GLenum usage_, GLuint totalSize_, GLuint verticesPerEntity );


        /***
         * 2. Drawing
         ***/
        virtual void drawEntity( GLuint index, GLfloat* color = nullptr ) = 0;
};

} // namespace gpu_buf

#endif // COLOURED_ENTITIES_BUFFER_H
