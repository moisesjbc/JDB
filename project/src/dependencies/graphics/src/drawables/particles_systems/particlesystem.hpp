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

#ifndef PARTICLESYSTEM_HPP
#define PARTICLESYSTEM_HPP

#include "../drawable.hpp"
#include "../../dependencies/dependencies.hpp"
#include "particles_generation.hpp"
#include <vector>

namespace m2g {

// Attributes per vertex = 8 = 2D pos + 2D vel + 4D color.
const unsigned int N_ATTRIBUTES_PER_VERTEX = 2 + 2 + 4;

/*
    - VBO estático
        - con las posiciones originales de todas las partículas.
        - con la velocidad original de cada particula.
        - Colores originales de todas las partículas.
*/

class ParticleSystem : public Drawable
{
    private:
        std::vector< ParticlesGeneration > particlesGenerations_;

        glm::vec2 baseLine_[2];

        GLuint vao_;
        GLuint vbo_;

        const unsigned int VBO_SIZE_;

        static GLint mvpMatrixLocation;
        static GLint tLocation;
        static GLint dColorLocation;

    public:
        /***
         * 1. Initialization
         ***/
        ParticleSystem( unsigned int& nGenerations, unsigned int &nParticlesPerGeneration );


        /***
         * 2. Transformations
         ***/
        //virtual void translate( const float& tx, const float& ty );
        //virtual void moveTo( const float& x, const float& y );


        /***
         * 3. Collision test
         ***/
        virtual bool collide( const Drawable& b ) const ;
        virtual const std::vector<Rect>* getCollisionRects() const ;


        /***
         * 4. Drawing
         ***/
        virtual void draw( const glm::mat4& projectionMatrix ) const = 0;
};




} // namespace m2g

#endif // PARTICLESYSTEM_HPP
