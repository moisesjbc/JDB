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

#include "level_intro.hpp"

namespace jdb {

/***
 * 1. Construction
 ***/

LevelIntro::LevelIntro( const GameState& parentGameState,
                        Window &window,
                        unsigned int levelIndex ) :
    GameState( window ),
    levelIndex_( levelIndex ),
    parentGameState_( parentGameState )
{}


/***
 * 2. GameState interface
 ***/

void LevelIntro::init()
{
    m2g::TextRenderer textRenderer( window_.renderer );
    SDL_Color fontColor = { 0, 0, 0, 255 };

    const unsigned int fontIndex =
            textRenderer.loadFont(
                "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf",
                30 );

    char text[250];
    sprintf( text, "Level %u\nPress any key to start", levelIndex_ );

    levelIntroText_ =
            textRenderer.drawText( text,
                                   fontIndex,
                                   fontColor );
    levelIntroText_->moveTo(
                ( ( window_.width() - levelIntroText_->getWidth() ) >> 1 ) +
                ( ( window_.width() - levelIntroText_->getWidth() ) >> 2 ),
                ( window_.height() - levelIntroText_->getHeight() ) >> 1 );
}


void LevelIntro::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent( &event );

    if( event.type == SDL_QUIT ){
        exit( 0 );
    }else if( event.type == SDL_KEYDOWN ){
        requestStateExit();
    }
}

void LevelIntro::update()
{

}


void LevelIntro::draw() const
{
    parentGameState_.draw();
    levelIntroText_->draw();
}


void LevelIntro::pause()
{

}


void LevelIntro::resume()
{

}


} // namespace jdb
