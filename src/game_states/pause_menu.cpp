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

#include "pause_menu.hpp"
#include <m2g/gui/widgets/text_button.hpp>

namespace jdb {

/***
 * 1. Construction
 ***/

PauseMenu::PauseMenu( Window& window ) :
    GameState( window ),
    gui_( window.renderer,
    {
          0,
          0,
          static_cast< unsigned int >( window.width() ),
          static_cast< unsigned int >( window.height() )
    })
{}


/***
 * 3. GameState interface
 ***/

void PauseMenu::init()
{
    m2g::FontInfo normalButtonFont =
    {
        "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf",
        30,
        { 0, 0, 0, 255 }
    };
    m2g::FontInfo hoverButtonFont =
    {
        normalButtonFont.path,
        35,
        { 150, 150, 150, 255 }
    };
    m2g::FontInfo pressedButtonFont =
    {
        normalButtonFont.path,
        35,
        { 200, 200, 200, 255 }
    };
    std::array< m2g::FontInfo, 3 > fontsInfo =
        { normalButtonFont,
          hoverButtonFont,
          pressedButtonFont };

    m2g::TextButtonPtr resumeButton( new m2g::TextButton( window_.renderer, "Resume game", fontsInfo ) );
    resumeButton->setPressCallback( [this](){
        requestStateExit();
    });

    m2g::TextButtonPtr returnToMenuButton(
                new m2g::TextButton(
                    window_.renderer,
                    "Return to main menu\nNOTE: Game progress won't be saved in this demo!",
                    fontsInfo ) );
    returnToMenuButton->setPressCallback( [this](){
        requestStateExit( -1 );
    });

    gui_.addWidget( std::move( resumeButton ) );
    gui_.addWidget( std::move( returnToMenuButton ) );

    SDL_ShowCursor( SDL_ENABLE );
}


void PauseMenu::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent( &event );

    if( event.type == SDL_QUIT ){
        exit( 0 );
    }

    gui_.handleEvent( event );
}


void PauseMenu::update()
{

}


void PauseMenu::draw() const
{
    SDL_SetRenderDrawColor( window_.renderer, 0xDC, 0xF1, 0xF1, 0xFF );
    SDL_RenderClear( window_.renderer );

    gui_.draw();

    SDL_RenderPresent( window_.renderer );
}


void PauseMenu::pause()
{

}


void PauseMenu::resume()
{
    SDL_ShowCursor( SDL_ENABLE );
}


} // namespace jdb
