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

#ifndef DANGERSTATE_HPP
#define DANGERSTATE_HPP

#include <tinyxml2.h>
#include "player_action_response.hpp"
#include <vector>
#include <set>

namespace jdb {

// TODO: Move to another file?
enum class ToolType
{
    HAND = 0,
    EXTINGUISHER,
    LIGHTER,
    GAVEL
};

enum class TauntType
{
    NONE = 0,
    BURN,
    FREEZING,
    ELECTROCUTION,
    BITE
};

struct DangerState
{
    // Index of the animation state associated with this state.
    int animationState;

    // Respones to player actions.
    std::vector< PlayerActionResponse > playerActionResponses;

    std::set< ToolType > tauntedTools;
    TauntType tauntType;


    /***
     * 1. Initialization
     ***/
    DangerState( tinyxml2::XMLElement* xmlElement );
};

} // namespace jdb.

#endif // DANGERSTATE_HPP
