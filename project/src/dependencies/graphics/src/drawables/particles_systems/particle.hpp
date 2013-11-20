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

#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <glm/glm.hpp>
#include "../../dependencies/dependencies.hpp"

namespace m2g {

struct Particle
{
    glm::vec4 dColor;
    unsigned int t;
    unsigned int life;
};


/*
- VBO con los vertices originales de todas las partículas.
- A las partículas les aplicaré solo traslaciones, crear un vector uniform vec2 translate y pasarlo por cada partícula.
    - Así no puedo dibujar todo de una sentada.
- ¿Usar pointsprites?
- ¿Cómo paso los colores?
- Renderizar a framebuffer.
-

- Puedo guardar en un buffer VBO todos los valores: pos, vec, minRGBA, maxRGBA.
- En un buffer especial donde pueda escribir puedo poner la vida restante de cada partícula.
- Dibujo todo de una sentada, y en el propio shader hago pos = orig + vel * life, color = color0 + vel * life, etc y actualizo life en su buffer.
- ¿O guardo en las memorias "escribibles" todos los campos actualizables como pos, color, etc y en VBO meto vel, etc.

- OLVIDAR LAS OPTIMIZACIONES.
    - VBO estático
        - con las posiciones originales de todas las partículas.
        - con la velocidad original de cada particula.
        - Colores originales de todas las partículas.

    - Uniform
        - El dColor se le pasa en un vec4.
        - La vida se le pasa en un uint. -> Hay que hacer un t.

    - El vertex shader hace pos = pos + vel * t;
    - El fragment shader hace color = color + dColor * t;


    - Cada generacion de particulas:
        - Conjunto de particulas que nacen en el mismo fotograma.
        -
*/

} // namespace m2g


#endif // PARTICLE_HPP
