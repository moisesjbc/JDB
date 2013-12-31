/***
    Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)

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

#ifndef SANDWICH_DATA_HPP
#define SANDWICH_DATA_HPP

#include <memory>
#include <vector>
#include "../dependencies/m2g/src/drawables/animation_data.hpp"
#include "../dependencies/m2g/src/graphics_library.hpp"
#include <iostream>
#include "../utilities/base_line.hpp"

namespace jdb {

struct SandwichData
{
    m2g::AnimationDataPtr animationData;

    BaseLine baseLine;


    /*** Methods ***/

    /***
     * 1. Initialization
     ***/
    SandwichData( tinyxml2::XMLElement* xmlElement, const m2g::GraphicsLibrary& graphicsLibrary );


    /***
     * 2. Loading
     ***/
    void loadFromXML( tinyxml2::XMLElement* xmlElement, const m2g::GraphicsLibrary& graphicsLibrary );
};

typedef std::shared_ptr< const SandwichData > SandwichDataPtr;

}

#endif // SANDWICH_DATA_HPP
