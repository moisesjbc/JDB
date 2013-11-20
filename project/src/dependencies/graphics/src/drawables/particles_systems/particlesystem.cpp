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

#include "particlesystem.hpp"

namespace m2g {


GLint ParticleSystem::mvpMatrixLocation = -1;
GLint ParticleSystem::tLocation = -1;
GLint ParticleSystem::dColorLocation = -1;


/***
 * 1. Initialization
 ***/

ParticleSystem::ParticleSystem( unsigned int& nGenerations, unsigned int &nParticlesPerGeneration ) :
    VBO_SIZE_( N_ATTRIBUTES_PER_VERTEX * nGenerations * nParticlesPerGeneration * sizeof( GLfloat ) )
{
    GLint currentProgram = 0;

    GLfloat* vertexData = nullptr;
    unsigned int i = 0;

    // Initializa the particles vector.
    ParticlesGeneration particlesGeneration( nParticlesPerGeneration, 0 );
    for( i = 0; i < nGenerations; i++ ){
        particlesGeneration.t = -i;
        particlesGenerations_.push_back( particlesGeneration );
    }

    // Initialize the base line.
    baseLine_[0] = glm::vec2( 25.0f, 200.0f );
    baseLine_[1] = glm::vec2( 125.0f, 200.0f );

    // Generate the VAO associated with this particles system and bind it as
    // the active one.
    glGenVertexArrays( 1, &vao_ );
    glBindVertexArray( vao_ );

    // Generate the VBO asociated with this particles systen, bind it as the
    // active one and allocate the requested space.
    glGenBuffers( 1, &vbo_ );
    glBindBuffer( GL_ARRAY_BUFFER, vbo_ );
    glBufferData( GL_ARRAY_BUFFER, VBO_SIZE_, NULL, GL_STATIC_DRAW );

    // Set vertex attribute pointers.

    // We are sending 2D vertices to the vertex shader.
    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, N_ATTRIBUTES_PER_VERTEX * sizeof(GLfloat), (GLvoid*)(0) );
    glEnableVertexAttribArray( 0 );

    // We are sending 2D velocity vectors to the shader.
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, N_ATTRIBUTES_PER_VERTEX * sizeof(GLfloat), (GLvoid*)(2*sizeof(GLfloat)) );
    glEnableVertexAttribArray( 1 );

    // We are sending RGBA colors to the shader.
    glVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, N_ATTRIBUTES_PER_VERTEX * sizeof(GLfloat), (GLvoid*)(4*sizeof(GLfloat)) );
    glEnableVertexAttribArray( 2 );

    // Map the VBO memory for writting.
    vertexData = reinterpret_cast< GLfloat* >( glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY ) );

    // Set the vertex data for every particle in the system.
    for( i = 0; i < N_ATTRIBUTES_PER_VERTEX * nGenerations * nParticlesPerGeneration; i += N_ATTRIBUTES_PER_VERTEX ){
        // Position.
        vertexData[i] = rand() % (int)( baseLine_[1].x - baseLine_[0].x ) + baseLine_[0].x;
        vertexData[i+1] = baseLine_[0].y;

        // Velocity.
        vertexData[i+2] = 1.0f;
        vertexData[i+3] = 0.0f;

        // Color.
        vertexData[i+4] = ( ( rand() % 255 ) / 255.0f );
        vertexData[i+5] = ( ( rand() % 255 ) / 255.0f );
        vertexData[i+6] = ( ( rand() % 255 ) / 255.0f );
        vertexData[i+7] = ( ( rand() % 255 ) / 255.0f );
    }

    // Unmap the VBO memory.
    glUnmapBuffer( GL_ARRAY_BUFFER );

    // If unset, set the GPU uniforms' locations.
    if( mvpMatrixLocation == -1 ){
        glGetIntegerv( GL_CURRENT_PROGRAM, &currentProgram );

        mvpMatrixLocation = glGetUniformLocation( currentProgram, "mvpMatrix" );
        tLocation = glGetUniformLocation( currentProgram, "tLocation" );
        dColorLocation = glGetUniformLocation( currentProgram, "dColor" );

        checkOpenGL( "Particles system () - Setting uniform locations" );
    }

    checkOpenGL( "Particles System constructor" );
}


/***
 * 3. Collision test
 ***/

bool ParticleSystem::collide( const Drawable& b ) const
{
    b.getX();
    // FIXME: Use the boundary box in future versions.
    return false;
}

const std::vector<Rect>* ParticleSystem::getCollisionRects() const
{
    return nullptr;
}


/***
 * 4. Drawing
 ***/

void ParticleSystem::draw( const glm::mat4& projectionMatrix ) const
{
    unsigned int i = 0, j = 0;

    // Bind the VAO and VBO of this particle system as the active ones.
    glBindVertexArray( vao_ );
    glBindBuffer( GL_ARRAY_BUFFER, vbo_ );

    // Send the MVP matrix to the shader.
    glUniformMatrix4fv( mvpMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0] );

    for( i = 0; i < particlesGenerations_.size(); i++ ){
        // Send the current generation's t to the shader.
        glUniform1i( tLocation, particlesGenerations_[i].t );

        for( j = 0; j < particlesGenerations_[i].particles.size(); j++ ){
            // Send the currents particle's t and dColor to the shader.
            glUniform4fv( dColorLocation, 1, &(particlesGenerations_[i].particles[j].dColor[0]) );

            // Draw every particle as a point.
            glDrawArrays( GL_POINTS, i * particlesGenerations_[0].particles.size() + j, 1 );
        }
    }
}

} // namespace m2g
