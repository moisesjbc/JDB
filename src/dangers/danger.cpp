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

#include "danger.hpp"

namespace jdb {

Danger::Danger( DangerDataPtr dangerData_ ) :
    Animation( dangerData_->animationData[ rand() % dangerData_->animationData.size()] )
{
    setDangerData( dangerData_ );
}


/***
 * 2. Getters
 ***/

float Danger::getDamage() const
{
    return hp * dangerData->damageFactor;
}


DangerDataPtr Danger::getDangerData() const
{
    return dangerData;
}

/***
 * 3. Setters
 ***/
// TODO: Overload Animation setters.
void Danger::setDangerData( DangerDataPtr dangerData_ )
{
    dangerData = dangerData_;

    setAnimationData( dangerData->animationData[0] );

    reset();
}

void Danger::setState( int newState )
{
    state = newState;

    setAnimationState( (dangerData->states[newState]).animationState );
}


/***
 * 4. Updating
 ***/

bool Danger::playerAction( PlayerAction playerAction, unsigned int& score )
{
    unsigned int i = 0;
    const PlayerActionResponse* playerActionResponse;
    std::vector< PlayerActionResponse > validPlayerResponses;

    // Iterate over the current state's vector of player action responses.
    while( i < dangerData->states[state].playerActionResponses.size() ){

        // Get the current player action response.
        playerActionResponse = &( dangerData->states[state].playerActionResponses[i] );

        // Check if the player action and the current danger's state meets the
        // conditions for applying this response.
        if( ( playerAction == playerActionResponse->playerAction ) &&
              ( hp >= playerActionResponse->minHp ) &&
              ( hp <= playerActionResponse->maxHp ) ){
            validPlayerResponses.push_back( *playerActionResponse );
        }
        i++;
    }


    // If we found one or more valid action responses, we randomly select one
    // and apply it here.
    if( validPlayerResponses.size() > 0 ){
        playerActionResponse = &validPlayerResponses[ rand() % validPlayerResponses.size() ];

        if( playerActionResponse->hpVariation == HP_ALL ){
            hp = 0;
        }else{
            if( playerActionResponse->dangerPerSecond ){
                hp += playerActionResponse->hpVariation * FPS_INVERSE;
            }else{
                hp += playerActionResponse->hpVariation;
            }
        }

        // TODO: Use hp variation instead.
        score += 10;
        score += playerActionResponse->scoreBonus;

        if( playerActionResponse->newState != -1 ){
            setState( playerActionResponse->newState );
        }

        return true;
    }

    return false;
}


void Danger::reset()
{
    hp = dangerData->initialHp;
    setState( dangerData->initialState );
}


TauntType Danger::taunts( const m2g::Sprite &tool, ToolType toolType ) const
{
    (void)( toolType );
    if( collide( tool ) && dangerData->states[ state ].tauntedTools.count( toolType ) ){
        return dangerData->states[ state ].tauntType;
    }
    return TauntType::NONE;
}

} // namespace jdb
