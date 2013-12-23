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

#include "tool.hpp"
#include <map>

namespace jdb {

/***
 * 1. Initialization
 ***/

Tool::Tool( m2g::AnimationDataPtr animationData ) :
    Animation( animationData )
{
    active_ = false;
    setToolType( ToolType::HAND );
}


/***
 * 3. Setters
 ***/

void Tool::setToolType( ToolType toolType )
{
    currentToolType_ = toolType;

    if( !active_ ){
        setAnimationState( 2 * static_cast< int >( currentToolType_  ) );
    }else{
        setAnimationState( 2 * static_cast< int >( currentToolType_  ) + 1 );
    }
}


/***
 * 4. Handlers
 ***/

void Tool::handleMouseButtonDown( Sandwich** sandwiches, unsigned int nSandwiches )
{
    unsigned int i = 0;

    switch( currentToolType_ ){
        case ToolType::HAND:
            while( ( i < nSandwiches ) &&
                   ( !sandwiches[i]->useTool( PlayerAction::HAND_CLICK, this ) ) ){
                i++;
            }
        break;
        case ToolType::GAVEL:
            while( ( i < nSandwiches ) &&
                   ( !sandwiches[i]->useTool( PlayerAction::GAVEL_HIT, this ) ) ){
                i++;
            }
        break;
    }

    active_ = true;
    setAnimationState( getAnimationState() + 1 );
}


void Tool::handleMouseButtonUp()
{
    active_ = false;
    setAnimationState( getAnimationState() - 1 );
}


void Tool::handleMouseHover( Sandwich** sandwiches, unsigned int nSandwiches )
{
    unsigned int i = 0;

    std::map< ToolType, PlayerAction > toolAction
    {
        { ToolType::EXTINGUISHER, PlayerAction::EXTINGUISHER_ON },
        { ToolType::LIGHTER, PlayerAction::LIGHTER_ON }
    };

    if( active_ ){
        if( ( currentToolType_ == ToolType::EXTINGUISHER ) ||
            ( currentToolType_ == ToolType::LIGHTER ) ){
            while( ( i < nSandwiches ) &&
                   ( !sandwiches[i]->useTool( toolAction[currentToolType_], this ) ) ){
                i++;
            }
        }
    }
}

} // namespace jdb
