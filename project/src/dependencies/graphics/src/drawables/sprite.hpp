/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of M2G.
 *
 * M2G is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * M2G is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with M2G.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "tileset.hpp"
#include "../utilities/tilesets_buffer.hpp"
#include <memory>
#include <iostream>
#include <vector>

namespace m2g {

class Sprite : public Drawable
{
    private:
        static GLint mvpMatrixLocation;
        static GLint samplerLocation;
        static GLint sliceLocation;

        GLuint vao;
        std::shared_ptr< Tileset > tileset;
        GLuint currentTile;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        Sprite();
        ~Sprite();
        void setTileset( std::shared_ptr< Tileset > tileset );
        const std::shared_ptr< Tileset > getTileset();



        /***
         * 2. VAO management
         ***/
        void initializeVAO();


        /***
         * 3. Current tile management
         ***/
        void nextTile();
        void previousTile();
        void setTile( const GLuint tile );
        GLuint getCurrentTile() const ;


        /***
         * 4. Collision detection
         ***/
    private:
        virtual const std::vector<Rect>* getCollisionRects() const ;
    public:


        /***
         * 5. Drawing
         ***/
        virtual void draw( const glm::mat4& projectionMatrix ) const ;


        /***
         * 6. Auxiliar methods
         ***/
    private:
        virtual void sendMVPMatrixToShader( const glm::mat4& mvpMatrix ) const ;
};


} // Namespace m2g

#endif // SPRITE_HPP
