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

#ifndef TOOL_HPP
#define TOOL_HPP

#include "../dependencies/m2g/src/drawables/animation.hpp"

#include "../sandwiches/sandwich.hpp"
#include "../dangers/danger.hpp"
#include <SDL2/SDL.h>

namespace jdb {

enum class ToolType
{
    HAND = 0,
    EXTINGUISHER
};


class Tool : public m2g::Animation
{
    private:
        ToolType currentToolType_;
        bool active_;

    public:
        /***
         * 1. Initialization
         ***/
        Tool( m2g::AnimationDataPtr animationData );


        /***
         * 2. Getters
         ***/


        /***
         * 3. Setters
         ***/
        void setToolType( ToolType toolType );


        /***
         * 4. Handlers
         ***/
        void handleMouseButtonDown( Sandwich** sandwiches, unsigned int nSandwiches );
        void handleMouseButtonUp();
        void handleMouseHover( Sandwich** sandwiches, unsigned int nSandwiches );
};

} // namespace jdb

#endif // TOOL_HPP
