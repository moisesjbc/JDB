/***
    Copyright 2013 - 2016 Moises J. Bonilla Caraballo (Neodivert)

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

#ifndef DANGER_DATA_HPP
#define DANGER_DATA_HPP

#include "danger_state.hpp"
#include <m2g/drawables/animation_data.hpp>
#include <m2g/graphics_library.hpp>
#include <vector>
#include <iostream>
#include "../utilities/base_line.hpp"

namespace jdb {

struct DangerData;
typedef std::shared_ptr< const DangerData > DangerDataPtr;

struct DangerData
{
    // Vector of animation data.
    std::vector< m2g::AnimationDataPtr > animationData;

    // Initial state and hp of the danger.
    int initialState;
    float initialHp;

    // When the Danger gets to the end of the conveyor belt, the damage done to Jacob
    // is equal to danger's hp * damageFactor.
    float damageFactor;

    BaseLine baseLine;

    // Vector of danger states.
    std::vector< DangerState > states;

    const std::vector< DangerDataPtr >& dangersDataVector;


    /***
     * 1. Initialization
     ***/
    DangerData( tinyxml2::XMLElement* xmlElement,
                m2g::GraphicsLibrary& dangerGraphics,
                const std::vector< DangerDataPtr >& dangersDataVector );
    DangerData(std::vector<m2g::AnimationDataPtr> animationData,
               const std::vector< DangerDataPtr >& dangersDataVector);
};

} // namespace jdb

#endif // DANGER_DATA_HPP
