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

#ifndef RECTS_BUFFER_HPP
#define RECTS_BUFFER_HPP

#include "entities_buffer.hpp"

namespace gpu_buf {

class RectsBuffer : public EntitiesBuffer
{
    private:
        unsigned int nRects;

    public:
        /***
         * 1. Initialization and destruction.
         ***/
        RectsBuffer( GLuint nRects, GLuint shaderVertexLocation );


        /***
         * 2. Data insertion
         ***/
        unsigned int insertRect( GLfloat x, GLfloat y, GLfloat w, GLfloat h );


        /***
         * 3. Drawing
         ***/
        virtual void drawAll( GLfloat* color = nullptr );
        virtual void drawEntity( GLuint index, GLfloat* color = nullptr );


        /***
         * 4. Auxiliar methods
         ***/
        virtual void clear();
};

} // namespace gpu_buf

#endif // RECTS_BUFFER_HPP
