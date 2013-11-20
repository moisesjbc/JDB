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

#ifndef PARTICLES_GENERATION_HPP
#define PARTICLES_GENERATION_HPP

#include "particle.hpp"
#include <vector>

namespace m2g {

struct ParticlesGeneration
{
    unsigned int t;
    std::vector< Particle > particles;


    /***
     * 1. Initialization and destruction.
     ***/
    ParticlesGeneration( unsigned int nParticles, unsigned int t_ );
};

} // namespace m2g

#endif // PARTICLES_GENERATION_HPP
