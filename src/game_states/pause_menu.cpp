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

#include <game_states/pause_menu.hpp>
#include <SFML/Window/Event.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <string>
#include <functional>
#include <paths.hpp>
#include <utilities/volume_control_panel.hpp>

const float SPACE_BETWEEN_WIDGETS = 0.005f;

namespace jdb {

/***
 * 1. Construction
 ***/

PauseMenu::PauseMenu( sf::RenderWindow& window, const GameState& parentGameState ) :
    GameState( window ),
    parentGameState_( parentGameState ),
    gui_( window )
{}


/***
 * 3. GameState interface
 ***/

void PauseMenu::init()
{
    gui_.setFont( DATA_DIR_PATH + "/fonts/LiberationSans-Bold.ttf" );

    tgui::VerticalLayout::Ptr pausePanel = std::make_shared<tgui::VerticalLayout>();
    pausePanel->setSize( tgui::bindWidth( gui_ ) * 0.30f,
                         tgui::bindHeight( gui_ ) * 0.30f );
    pausePanel->setPosition( tgui::bindWidth( gui_ ) * 0.35f,
                             tgui::bindHeight( gui_ ) * 0.35f );
    pausePanel->addSpace(SPACE_BETWEEN_WIDGETS);

    pausePanel->setBackgroundColor( sf::Color::White );

    tgui::Label::Ptr pauseMenuLabel = std::make_shared<tgui::Label>();
    pauseMenuLabel->setText( "Game paused" );
    pauseMenuLabel->setTextColor( sf::Color::Black );
    pausePanel->add( pauseMenuLabel );
    pausePanel->insertSpace( 30, SPACE_BETWEEN_WIDGETS );

    createPauseMenuButtons( pausePanel, pauseMenuLabel );

    VolumeControlPanel::Ptr volumeControlPanel = std::make_shared<VolumeControlPanel>();
    pausePanel->add( volumeControlPanel );
    pausePanel->setRatio( volumeControlPanel, 2.0f );
    pausePanel->insertSpace( 30, SPACE_BETWEEN_WIDGETS );

    gui_.add( pausePanel );

    window_.setMouseCursorVisible( true );
}


void PauseMenu::handleEvents()
{
    sf::Event event;

    while( window_.pollEvent( event ) ){
        if( event.type == sf::Event::Closed ){
            requestGameExit();
        }else{
            gui_.handleEvent( event );
        }
    }
}


void PauseMenu::update( unsigned int ms )
{
    (void)( ms );
}


void PauseMenu::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw( parentGameState_, states );
    gui_.draw();
}


void PauseMenu::pause()
{

}


void PauseMenu::resume()
{
    window_.setMouseCursorVisible( true );
}


/***
 * 4. Initialization auxiliar methods
 ***/

void PauseMenu::createPauseMenuButtons( tgui::VerticalLayout::Ptr menuPanel,
                                        tgui::Widget::Ptr menuLabel )
{
    struct PauseMenuButtonData
    {
        std::string text;
        std::function<void()> callback;
    };
    std::vector< PauseMenuButtonData > pauseMenuButtonsData =
    {
        { "Resume game", [this](){ requestStateExit(); } },
        { "Return to main menu", [this](){ requestStateExit( RETURN_TO_MAIN_MENU_REQUESTED ); } }
    };
    tgui::Widget::Ptr upperWidget = menuLabel;
    for( const PauseMenuButtonData& buttonData : pauseMenuButtonsData ){
        tgui::Button::Ptr button = std::make_shared<tgui::Button>();
        button->setText( buttonData.text );
        button->setTextSize( 14 );
        button->connect( "pressed", buttonData.callback );

        menuPanel->add( button );
        menuPanel->insertSpace( 30, SPACE_BETWEEN_WIDGETS );

        upperWidget = button;
    }
}


} // namespace jdb
