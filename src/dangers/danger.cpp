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

#include <dangers/danger.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace jdb {

Danger::Danger( DangerDataPtr dangerData_,
                m2g::AnimationData* appearanceAnimationData ) :
    Animation( *( dangerData_->animationData[rand() % dangerData_->animationData.size()] ) )
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


bool Danger::onScreen() const
{
    return (getPosition().x + dangerData->baseLine.x) < WINDOW_WIDTH;
}


/***
 * 3. Setters
 ***/
// TODO: Overload Animation setters.
void Danger::setDangerData( DangerDataPtr dangerData_,
                            m2g::AnimationData* appearanceAnimationData )
{
    dangerData = dangerData_;

    if( appearanceAnimationData != nullptr ){
        appearanceAnimation =
                std::unique_ptr< m2g::Animation >(
                    new m2g::Animation( *appearanceAnimationData ) );
        appearanceAnimation->move( getBoundaryBox().left + ( getBoundaryBox().width - appearanceAnimation->getBoundaryBox().width ) / 2,
                                   getBoundaryBox().top + ( getBoundaryBox().height - appearanceAnimation->getBoundaryBox().height ) / 2 );
    }

    setAnimationData( *( dangerData->animationData[0] ) );

    reset();
}


void Danger::setDangerDataWithUniqueAnimationData(DangerDataPtr dangerData_, m2g::AnimationDataPtr appearanceAnimationData)
{
    dangerData = dangerData_;

    if( appearanceAnimationData != nullptr ){
        appearanceAnimation =
                std::unique_ptr< m2g::Animation >(
                    new m2g::Animation( std::move(appearanceAnimationData) ) );
        appearanceAnimation->move( getBoundaryBox().left + ( getBoundaryBox().width - appearanceAnimation->getBoundaryBox().width ) / 2,
                                   getBoundaryBox().top + ( getBoundaryBox().height - appearanceAnimation->getBoundaryBox().height ) / 2 );
    }

    setAnimationData( *( dangerData->animationData[0] ) );

    reset();
}


void Danger::setState( int newState )
{
    state = newState;

    Animation::setState( (dangerData->states[newState]).animationState );
}


/***
 * 4. Updating
 ***/

void Danger::update( unsigned int ms,
                     int& playerHp,
                     unsigned int& playerScore,
                     m2g::GraphicsLibrary& dangersGraphicsLibrary,
                     float difficultyFactor )
{
    m2g::Animation::update( ms );
    if( appearanceAnimation ){
        appearanceAnimation->update( ms );
    }

    // Check if we must change the current danger when the current animation
    // state ends, and apply it if applicable.
    if( dangerData->states[state].randomDangerOnAnimationStateEnd &&
        this->Animation::finished() ){

        // FIXME: Duplicated code.
        DangerDataPtr newDangerData =
                dangerData->dangersDataVector[ rand() % dangerData->dangersDataVector.size() ];
        move( dangerData->baseLine.x + ( dangerData->baseLine.width - newDangerData->baseLine.width ) / 2 - newDangerData->baseLine.x,
                   dangerData->baseLine.y - newDangerData->baseLine.y );
        setDangerData( newDangerData, dangerData->states[state].appearanceAnimationData.get() );
    }

    // Check if we have any time-based state transition and apply it if
    // applicable.
    static unsigned int timeElapsedFromLastTimeout = 0;
    if( dangerData->states[state].stateTimeTransition != nullptr && onScreen() ){
        static unsigned int currentTimeout =
                dangerData->states[state].stateTimeTransition->generateTimeout();

        if( ms + timeElapsedFromLastTimeout >= currentTimeout ){
            timeElapsedFromLastTimeout = 0;
            applyMutation(dangerData->states[state].stateTimeTransition->dangerMutation, playerHp, playerScore, dangersGraphicsLibrary, difficultyFactor);
        }else{
            timeElapsedFromLastTimeout += ms;
        }
    }


    // Check if we have any distance-based state transition and apply it if
    // applicable.
    if( dangerData->states[state].stateDistanceTransition != nullptr ){
        if( this->getBoundaryBox().left < static_cast< int >( dangerData->states[state].stateDistanceTransition->distance ) ){
            applyMutation( dangerData->states[state].stateDistanceTransition->dangerMutation, playerHp, playerScore, dangersGraphicsLibrary, difficultyFactor );
        }
    }
}


bool Danger::playerAction( PlayerAction playerAction,
                           int& playerHp,
                           unsigned int& playerScore,
                           m2g::GraphicsLibrary& dangersGraphicsLibrary,
                           float difficultyFactor )
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

        applyMutation(playerActionResponse->dangerMutation, playerHp, playerScore, dangersGraphicsLibrary, difficultyFactor);

        return true;
    }

    return false;
}


void Danger::applyMutation(const DangerMutation &mutation,
                           int& playerHp,
                           unsigned int& playerScore,
                           m2g::GraphicsLibrary& dangersGraphicsLibrary,
                           float difficultyFactor )
{
    int oldDangerHp = hp;
    if(mutation.newDanger() != DANGER_NULL_ID){
        DangerDataPtr newDangerData =
            *std::find_if(dangerData->dangersDataVector.begin(), dangerData->dangersDataVector.end(), [=](DangerDataPtr currentDangerData){
                return currentDangerData->id == mutation.newDanger();
            });
        move( dangerData->baseLine.x + ( dangerData->baseLine.width - newDangerData->baseLine.width ) / 2 - newDangerData->baseLine.x,
                   dangerData->baseLine.y - newDangerData->baseLine.y );
        setDangerDataWithUniqueAnimationData(newDangerData, std::move(dangersGraphicsLibrary.getAnimationDataByName(mutation.newDangerAppearanceAnimaton())));
    }

    if(mutation.dangerHpVariation() == HP_ALL){
        hp = 0;
    }else{
        hp += mutation.dangerHpVariation();
    }

    playerHp += mutation.playerHpVariation();

    int playerScoreVariation = mutation.playerScoreVariation();
    if( hp < oldDangerHp){
        playerScoreVariation += (oldDangerHp - hp) * dangerData->damageFactor;
    }
    playerScoreVariation *= difficultyFactor;
    if( (int)playerScore + playerScoreVariation > 0 ){
        playerScore += playerScoreVariation;
    }else{
        playerScore = 0;
    }

    if(mutation.newDangerState() != -1){
        setState(mutation.newDangerState());
    }
}


void Danger::reset()
{
    hp = dangerData->initialHp;
    setState( dangerData->initialState );
}


StunType Danger::stuns( const m2g::TileSprite &tool, ToolType toolType ) const
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

void Danger::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    Animation::draw( target, states );

    if( appearanceAnimation ){
        target.draw( *appearanceAnimation, states );
    }
}

} // namespace jdb
