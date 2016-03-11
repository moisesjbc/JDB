/***
    Copyright 2013 - 2016 Moises J. Bonilla Caraballo (moisesjbc)

    This file is part of sandwiches-game.

    sandwiches-game is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    sandwiches-game is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with sandwiches-game.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#ifndef DANGERSTATE_HPP
#define DANGERSTATE_HPP

#include <tinyxml2.h>
#include "player_action_response.hpp"
#include <vector>
#include <set>
#include <memory>
#include <m2g/drawables/animation_data.hpp>
#include <m2g/graphics_library.hpp>
#include <dangers/state_time_transition.hpp>
#include <dangers/state_distance_transition.hpp>
#include <tools/tool_type.hpp>
#include <dangers/stun_type.hpp>

namespace jdb {

struct DangerState
{
    // Index of the animation state associated with this state.
    int animationState;

    // Respones to player actions.
    std::vector< PlayerActionResponse > playerActionResponses;

    // State time-based transitions.
    std::unique_ptr< StateTimeTransition > stateTimeTransition;

    // Distance-based state transition.
    std::unique_ptr< StateDistanceTransition > stateDistanceTransition;

    bool randomDangerOnAnimationStateEnd;
    m2g::AnimationDataPtr appearanceAnimationData;

    std::set< ToolType > stunnedTools;
    StunType stunType;


    /***
     * 1. Initialization
     ***/
    DangerState() = default;
};

} // namespace jdb.

#endif // DANGERSTATE_HPP
