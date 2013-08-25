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

#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "drawable.hpp"
#include <memory>

namespace jdb {

struct Tileset {
    GLuint texture;
    GLuint vbo;

    Tileset() : texture(0), vbo(0) {}
    ~Tileset(){
        glDeleteBuffers( 1, &vbo );
        glDeleteTextures( 1, &texture );
    }
};

//std::shared_ptr<SpriteData> readSpriteDataFromFile( const char* filePath );


class Sprite : public Drawable
{
    private:
        static GLuint vao;
        static GLint mvpMatrixLocation;
        static GLint samplerLocation;
        static GLint sliceLocation;

        GLuint vbo;
        GLuint texture;
        std::shared_ptr< Tileset > tileset;

    public:
        /***
         * 1. Initialization
         ***/
        Sprite();
        void setTileset( const std::shared_ptr< Tileset >& tileset );

        static std::shared_ptr<Tileset> loadTileset( const tinyxml2::XMLNode* xmlNode );

        /***
         * 2. VAO management
         ***/
        static void initializeVAO();
        static GLuint getVAO();

        /***
         * 3. Drawing
         ***/
        virtual void draw( const glm::mat4& projectionMatrix ) const ;

        /***
         * 4. Auxiliar methods
         ***/
    private:
        virtual void sendMVPMatrixToShader( const glm::mat4& mvpMatrix ) const ;
};


} // Namespace jdb

#endif // SPRITE_HPP
