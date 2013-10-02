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

#include "animation_data.hpp"

namespace m2g {

/***
 * 1. Initialization and destruction
 ***/

AnimationData::AnimationData( const tinyxml2::XMLNode* xmlNode, const char* folder )
{
    load( xmlNode, folder );
}


/***
 * 2. Loading
 ***/

void AnimationData::load( const tinyxml2::XMLNode* xmlNode, const char* folder )
{
    std::array< int, 3 > animationState;

    const tinyxml2::XMLNode* tilesetNode = xmlNode->FirstChildElement( "tileset" );
    const tinyxml2::XMLNode* animationStatesNode = tilesetNode->NextSiblingElement( "animation_states" );
    const tinyxml2::XMLElement* animationStateNode = nullptr;

    // Load the tileset info.
    tileset = std::shared_ptr< Tileset >( new Tileset( tilesetNode, folder ) );

    // Access the XML node with the animation states info.
    animationStateNode = animationStatesNode->FirstChildElement();

    while( animationStateNode ){
        // Composite the new animation state.
        animationState[FIRST_FRAME] = animationStateNode->IntAttribute( "firstFrame" );
        animationState[LAST_FRAME] = animationStateNode->IntAttribute( "lastFrame" );
        animationState[BACK_FRAME] = animationStateNode->IntAttribute( "backFrame" );

        // Insert the new animation state in the vector.
        states.push_back( animationState );

        // Go to next node.
        animationStateNode = animationStateNode->NextSiblingElement();
    }
}

} // namespace m2g
