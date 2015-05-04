/***
    Copyright 2013 - 2015 Moises J. Bonilla Caraballo (Neodivert)

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
#include <SFML/Window/Mouse.hpp>

namespace jdb {

/***
 * 1. Initialization
 ***/

Tool::Tool( m2g::AnimationDataPtr animationData, SoundManager& soundManager ) :
    Animation( std::move( animationData ) ),
    soundManager_( soundManager )
{
    unsigned int i = 0;

    const std::string audioFolder = "data/audio/tools/";
    const std::string audioFiles[] =
    {
        "hand.ogg",
        "extinguisher.ogg",
        "lighter.ogg",
        "gavel_hit.ogg"
    };
    bool audioLoop[] =
    {
        false,
        true,
        false,
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
    soundBuffer_.loadFromFile( "data/audio/player/cry-1.ogg" );
    sound_.setBuffer( soundBuffer_ );
    sound_.setLoop( false );

    crySoundIndex_ = soundManager_.loadSounds( "player", "cry-" );
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
        setState( 2 * static_cast< int >( currentToolType_  ) );
    }else{
        setState( 2 * static_cast< int >( currentToolType_  ) + 1 );
    }
}


/***
 * 4. Handlers
 ***/

void Tool::handleMouseButtonDown( SandwichesVector& sandwiches,
                                  unsigned int& score,
                                  unsigned int& hpBonus )
{
    unsigned int i = 0;

    // Play the sound associated to the current tool.
    sounds_[ static_cast<int>(currentToolType_) ].play();

    if( currentToolType_ == ToolType::HAND ){
        while( ( i < sandwiches.size() ) &&
               ( !sandwiches[i]->useTool( PlayerAction::HAND_CLICK, this, score, hpBonus ) ) ){
            i++;
        }
    }else if( currentToolType_ == ToolType::GAVEL ){
        while( ( i < sandwiches.size() ) &&
               ( !sandwiches[i]->useTool( PlayerAction::GAVEL_HIT, this, score, hpBonus ) ) ){
            i++;
        }
    }

    active_ = true;
    setState( currentState() + 1 );
}


void Tool::handleMouseButtonUp()
{
    if( currentState() % 2 ){
        active_ = false;
        setState( currentState() - 1 );

        // Stop the sound associated to the current tool.
        sounds_[ static_cast<int>(currentToolType_) ].stop();
    }
}


void Tool::handleMouseHover( SandwichesVector& sandwiches, unsigned int& score, unsigned int& hpBonus )
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
            while( ( i < sandwiches.size() ) &&
                   ( !sandwiches[i]->useTool( toolAction[currentToolType_], this, score, hpBonus ) ) ){
                i++;
            }
        }
    }
}


/***
 * 5. Auxiliar methods
 ***/

void Tool::applyStun( SandwichesVector& sandwiches )
{
    unsigned int i;
    StunType stunType;

    sf::Vector2i mousePos = sf::Mouse::getPosition();
    for( i = 0; i < sandwiches.size(); i++ ){
        stunType = sandwiches[i]->stuns( *this, currentToolType_ );
        if( stunType != StunType::NONE ){
            sf::Mouse::setPosition( sf::Vector2i( mousePos.x, mousePos.y - 75 ) );

            soundManager_.playSound( crySoundIndex_ );
        }
    }
}

} // namespace jdb
