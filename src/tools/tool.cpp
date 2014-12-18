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

#include "tool.hpp"
#include <map>

namespace jdb {

/***
 * 1. Initialization
 ***/

Tool::Tool( m2g::AnimationDataPtr animationData ) :
    Animation( animationData )
{
    unsigned int i = 0;

    const std::string audioFolder = "data/audio/tools/";
    const std::string audioFiles[] =
    {
        "gavel_hit.ogg",
        "extinguisher.ogg",
        "lighter.ogg",
        "gavel_hit.ogg"
    };
    bool audioLoop[] =
    {
        false,
        true,
        true,
        false
    };

    active_ = false;
    setToolType( ToolType::HAND );

    // Load the sounds.
    for( i = 0; i < 4; i++ ){
        if( !soundBuffers_[i].loadFromFile( ( audioFolder + audioFiles[i] ) ) ){
            throw std::runtime_error( std::string( "ERROR loading file [" ) + audioFolder + audioFiles[i] + std::string( "]" ) );
        }
        sounds_[i].setBuffer( soundBuffers_[i] );
        sounds_[i].setLoop( audioLoop[i] );
    }
}


/***
 * 3. Setters
 ***/

void Tool::setToolType( ToolType toolType )
{
    currentToolType_ = toolType;

    // Stop the sound associated to the current tool.
    sounds_[ static_cast<int>(currentToolType_) ].stop();

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

    // Play the sound associated to the current tool.
    sounds_[ static_cast<int>(currentToolType_) ].play();

    if( currentToolType_ == ToolType::HAND ){
        while( ( i < nSandwiches ) &&
               ( !sandwiches[i]->useTool( PlayerAction::HAND_CLICK, this ) ) ){
            i++;
        }
    }else if( currentToolType_ == ToolType::GAVEL ){
        while( ( i < nSandwiches ) &&
               ( !sandwiches[i]->useTool( PlayerAction::GAVEL_HIT, this ) ) ){
            i++;
        }
    }

    active_ = true;
    setAnimationState( getAnimationState() + 1 );
}


void Tool::handleMouseButtonUp()
{
    active_ = false;
    setAnimationState( getAnimationState() - 1 );

    // Stop the sound associated to the current tool.
    sounds_[ static_cast<int>(currentToolType_) ].stop();
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


/***
 * 5. Auxiliar methods
 ***/

void Tool::applyTaunt( Sandwich **sandwiches, unsigned int N_SANDWICHES )
{
    unsigned int i;
    TauntType tauntType;
    int mouseX, mouseY;

    SDL_PumpEvents();
    SDL_GetMouseState( &mouseX, &mouseY );

    for( i = 0; i < N_SANDWICHES; i++ ){
        tauntType = sandwiches[i]->taunts( *this, currentToolType_ );
        if( tauntType != TauntType::NONE ){
            SDL_WarpMouseInWindow( nullptr, mouseX, mouseY - 75 );
        }
    }
}

} // namespace jdb
