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

#include "main_menu.hpp"
#include <m2g/text/text_renderer.hpp>
#include "../levels/campaign_level.hpp"
#include "../levels/survival_level.hpp"
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

namespace jdb {

/***
 * 1. Creation
 ****/

MainMenu::MainMenu( Window &window, SoundManager* soundManager ) :
    GameState( window ),
    soundManager_( *soundManager )
{}


/***
 * 3. GameState interface
 ***/

void MainMenu::init()
{
    m2g::TextRenderer textRenderer( window_.renderer );
    const SDL_Color FONT_COLOR = { 131, 60, 60, 255 };

    // Create a sprite with the "menu".
    const unsigned int menuFontIndex =
            textRenderer.loadFont( "data/fonts/LiberationSans-Bold.ttf", 30 );

    menuText_ = textRenderer.drawText(
                "MENU\n---\nPRESS A KEY\n---\n\nC - CAMPAIGN MODE\nS - SURVIVAL MODE\nESC - EXIT",   // Text
                menuFontIndex,
                FONT_COLOR );
}


void MainMenu::handleEvents()
{
    SDL_Event event;
    std::unique_ptr< Level > level = nullptr;

    SDL_PollEvent( &event );
    if( event.type == SDL_QUIT ){
        requestStateExit();
    }else if( event.type == SDL_KEYDOWN ){
        switch( event.key.keysym.sym ){
            case SDLK_ESCAPE:
                requestStateExit();
            break;
            case SDLK_c:
                level = std::unique_ptr< Level >(
                           new CampaignLevel( window_, &soundManager_, 0 ) );
                level->run();
            break;
            case SDLK_s:
                level = std::unique_ptr< Level >(
                            new SurvivalLevel( window_, &soundManager_, 0 ) );
                level->run();
            break;
            default:
            break;
        }
    }
}


void MainMenu::update()
{

}


void MainMenu::draw()
{
    SDL_SetRenderDrawColor( window_.renderer, 0xDC, 0xF1, 0xF1, 0xFF );
    SDL_RenderClear( window_.renderer );

    menuText_->draw();

    SDL_RenderPresent( window_.renderer );
}

void MainMenu::pause()
{
}

void MainMenu::resume()
{

}

} // namespace jdb
