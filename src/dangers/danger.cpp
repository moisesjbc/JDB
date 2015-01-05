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

#include "danger.hpp"

namespace jdb {

Danger::Danger( DangerDataPtr dangerData_,
                const m2g::GraphicsLibrary& graphicsLibrary,
                m2g::AnimationDataPtr appearanceAnimationData ) :
    Animation( dangerData_->animationData[ rand() % dangerData_->animationData.size()] ),
    graphicsLibrary_( graphicsLibrary )
{
    setDangerData( dangerData_, appearanceAnimationData );
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
void Danger::setDangerData( DangerDataPtr dangerData_,
                            m2g::AnimationDataPtr appearanceAnimationData )
{
    dangerData = dangerData_;

    if( appearanceAnimationData != nullptr ){
        appearanceAnimation =
                std::unique_ptr< m2g::Animation >(
                    new m2g::Animation( appearanceAnimationData ) );
        appearanceAnimation->moveTo( getBoundaryBox()->x + ( getWidth() - appearanceAnimation->getWidth() ) / 2,
                                     getBoundaryBox()->y + ( getHeight() - appearanceAnimation->getHeight() ) / 2 );
    }

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

void Danger::update()
{
    m2g::Animation::update();
    if( appearanceAnimation ){
        appearanceAnimation->update();
    }

    // Check if we must change the current danger when the current animation
    // state ends, and apply it if applicable.
    if( dangerData->states[state].randomDangerOnAnimationStateEnd &&
        this->Animation::finished() ){

        // FIXME: Duplicated code.
        DangerDataPtr newDangerData =
                dangerData->dangersDataVector[ rand() % dangerData->dangersDataVector.size() ];
        translate( dangerData->baseLine.x + ( dangerData->baseLine.width - newDangerData->baseLine.width ) / 2 - newDangerData->baseLine.x,
                   dangerData->baseLine.y - newDangerData->baseLine.y );
        setDangerData( newDangerData,
                       graphicsLibrary_.getAnimationData( dangerData->states[state].appearanceAnimationLabel ) );
    }

    // Check if we have any time-based state transition and apply it if
    // applicable.
    static Uint32 lastTimeout = SDL_GetTicks();
    if( dangerData->states[state].stateTimeTransition != nullptr ){
        static Uint32 currentTimeout =
                dangerData->states[state].stateTimeTransition->generateTimeout();

        if( SDL_GetTicks() - lastTimeout >= currentTimeout ){
            lastTimeout = SDL_GetTicks();
            currentTimeout =
                    dangerData->states[state].stateTimeTransition->generateTimeout();
            setState( dangerData->states[state].stateTimeTransition->newState );
        }
    }


    // Check if we have any distance-based state transition and apply it if
    // applicable.
    if( dangerData->states[state].stateDistanceTransition != nullptr ){
        if( this->getBoundaryBox()->x < dangerData->states[state].stateDistanceTransition->distance ){
            setState( dangerData->states[state].stateDistanceTransition->newState );
        }
    }
}


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

        if( playerActionResponse->newDanger != -1 ){
            // FIXME: Duplicated code.
            DangerDataPtr newDangerData =
                    dangerData->dangersDataVector[ rand() % dangerData->dangersDataVector.size() ];
            translate( dangerData->baseLine.x + ( dangerData->baseLine.width - newDangerData->baseLine.width ) / 2 - newDangerData->baseLine.x,
                       dangerData->baseLine.y - newDangerData->baseLine.y );
            setDangerData( newDangerData );
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


StunType Danger::stuns( const m2g::Sprite &tool, ToolType toolType ) const
{
    (void)( toolType );
    if( collide( tool ) && dangerData->states[ state ].stunnedTools.count( toolType ) ){
        return dangerData->states[ state ].stunType;
    }
    return StunType::NONE;
}


/***
 * 5. Drawing
 ***/

void Danger::draw( const glm::mat4 &projectionMatrix ) const
{
    Animation::draw( projectionMatrix );
    if( appearanceAnimation ){
        appearanceAnimation->draw( projectionMatrix );
    }
}

} // namespace jdb
