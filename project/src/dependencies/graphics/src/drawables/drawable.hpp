/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of Graphic Librarian.
 *
 * Graphic Librarian is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Graphic Librarian is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Graphic Librarian.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include "../dependencies/dependencies.hpp"
#include "../dependencies/gpu-buffers/src/buffer.hpp"
#include <vector>

namespace m2g {

struct Rect
{
    GLfloat x;
    GLfloat y;
    GLfloat width;
    GLfloat height;
};

class Drawable
{
    protected:
        glm::vec2 position;

    public:
        /***
         * 1. Initialization
         ***/
        Drawable();


        /***
         * 2. Getters and setters
         ***/
        GLfloat getX() const;
        glm::vec2 getPosition() const ;


        /***
         * 3. Transformations
         ***/
        virtual void translate( const float& tx, const float& ty );


        /***
         * 4. Collision test
         ***/
        virtual bool collide( const Drawable& b ) const ;
    private:
        virtual const std::vector<Rect>* getCollisionRects() const = 0;


        /***
         * 5. Drawing
         ***/
        virtual void draw( const glm::mat4& projectionMatrix ) const = 0;
};


} // Namespace m2g

#endif // DRAWABLE_HPP
