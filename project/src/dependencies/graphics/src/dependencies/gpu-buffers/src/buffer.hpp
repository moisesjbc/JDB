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

#ifndef BUFFER_HPP
#define BUFFER_HPP

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

#include <GL/glu.h>
#include <iostream>

namespace gpu_buf {

class Buffer
{
    protected:
        // Buffer's associated VAO and VBO.
        GLuint vao;
        GLuint vbo;

        // Buffer's occupied and total sizes (in bytes).
        GLuint occupiedSize;
        GLuint totalSize;

        // Usage hint
        GLenum vboUsage;

        // VBO's target.
        GLenum vboTarget;

        // Location Shader uniform variable "color".
        GLint colorUniformLocation;

        // Default color for primitives
        GLfloat defaultColor[4];

    public:
        /***
         * 1. Initialization and destruction
         ***/
        Buffer( GLenum target_, GLenum usage_, GLuint totalSize_ );
        ~Buffer();


        /***
         * 2. Setters and getters
         ***/
        void setVertexAttribPointer( GLuint index,  GLint size,  GLenum type,  GLboolean normalized, GLsizei stride, const GLvoid * pointer );
        void setColorUniformLocation( GLint location );


        /***
         * 3. Data insertion.
         ***/
    protected:
        void insertData( GLvoid* newData, GLuint size );


        /***
         * 4. Drawing.
         ***/
    public:
        virtual void drawAll( GLfloat* color = nullptr ) = 0;


        /***
         * 5. Auxiliar methods
         ***/
        virtual void clear();
};

} // gpu_buf

#endif // BUFFER_HPP
