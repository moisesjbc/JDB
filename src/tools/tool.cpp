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

#include <tools/tool.hpp>
#include <map>
#include <SFML/Window/Mouse.hpp>
#include <paths.hpp>

namespace jdb {

/***
 * 1. Initialization
 ***/

Tool::Tool( m2g::AnimationDataPtr animationData, SoundManager& soundManager ) :
    Animation( std::move( animationData ) ),
    currentToolType_( ToolType::HAND ),
    soundManager_( soundManager )
{
    unsigned int i = 0;

    const std::string audioFolder = DATA_DIR_PATH + "/audio/tools/";
    const std::string audioFiles[] =
    {
        "hand.ogg",
        "extinguisher.ogg",
        "lighter.ogg",
        "gavel.ogg"
    };
    bool audioLoop[] =
    {
        false,
        true,
        false,
        false
    };

    // Load the sounds.
    for( i = 0; i < 4; i++ ){
        if( !soundBuffers_[i].loadFromFile( ( audioFolder + audioFiles[i] ) ) ){
            throw std::runtime_error( std::string( "ERROR loading file [" ) + audioFolder + audioFiles[i] + std::string( "]" ) );
        }
        sounds_[i].setBuffer( soundBuffers_[i] );
        sounds_[i].setLoop( audioLoop[i] );
    }
    soundBuffer_.loadFromFile( DATA_DIR_PATH + "/audio/player/cry-1.ogg" );
    sound_.setBuffer( soundBuffer_ );
    sound_.setLoop( false );

    crySoundIndex_ = soundManager_.loadSounds( "player", "cry-" );

    active_ = false;
    setToolType( ToolType::HAND );
}


/***
 * 2. Getters
 ***/

unsigned int Tool::index() const
{
    return static_cast<int>(currentToolType_);
}


/***
 * 3. Setters
 ***/

void Tool::setToolType( ToolType toolType )
{
    // Stop the sound associated to the current tool.
    sounds_[ static_cast<int>(currentToolType_) ].stop();

    currentToolType_ = toolType;

    if( !active_ ){
        setState( 2 * static_cast< int >( currentToolType_  ) );
    }else{
        setState( 2 * static_cast< int >( currentToolType_  ) + 1 );
    }
}


void Tool::setActive(bool active)
{
    if(active != active_){
        if(active){
            // Play the sound associated to the current tool.
            sounds_[ static_cast<int>(currentToolType_) ].setVolume( sf::Listener::getGlobalVolume() );
            sounds_[ static_cast<int>(currentToolType_) ].play();

            setState( currentState() + 1 );
        }else{
            sounds_[ static_cast<int>(currentToolType_) ].stop();

            setState( currentState() - 1 );
        }
        active_ = active;
    }
}


/***
 * 4. Handlers
 ***/

void Tool::handleMouseButtonDown( SandwichesVector& sandwiches,
                                  int& playerHp,
                                  unsigned int& playerScore,
                                  m2g::GraphicsLibrary& dangersGraphicsLibrary,
                                  float difficultyFactor)
{
    unsigned int i = 0;

    if( currentToolType_ == ToolType::HAND ){
        while( ( i < sandwiches.size() ) &&
               ( !sandwiches[i]->useTool( PlayerAction::HAND_CLICK, this, playerHp, playerScore, dangersGraphicsLibrary, difficultyFactor ) ) ){
            i++;
        }
    }else if( currentToolType_ == ToolType::GAVEL ){
        while( ( i < sandwiches.size() ) &&
               ( !sandwiches[i]->useTool( PlayerAction::GAVEL_HIT, this, playerHp, playerScore, dangersGraphicsLibrary, difficultyFactor ) ) ){
            i++;
        }
    }

    setActive(true);
}


void Tool::handleMouseButtonUp()
{
    if( currentState() % 2 ){
        setActive(false);
    }
}


void Tool::handleMouseHover( SandwichesVector& sandwiches, int& playerHp, unsigned int& playerScore, m2g::GraphicsLibrary& dangersGraphicsLibrary, float difficultyFactor )
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
                   ( !sandwiches[i]->useTool( toolAction[currentToolType_], this, playerHp, playerScore, dangersGraphicsLibrary, difficultyFactor ) ) ){
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
