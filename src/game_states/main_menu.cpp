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
#include "../levels/campaign_level.hpp"
#include "../levels/survival_level.hpp"
#include "credits_screen.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics/Font.hpp>
#include <utilities/volume_control_panel.hpp>

namespace jdb {

/***
 * 1. Creation
 ****/

MainMenu::MainMenu( sf::RenderWindow& window, SoundManager* soundManager ) :
    GameState( window ),
    soundManager_( *soundManager ),
    gui_( window )
{}


/***
 * 3. GameState interface
 ***/

void MainMenu::init()
{
    gui_.setFont( DATA_DIR_PATH + "/fonts/LiberationSans-Bold.ttf" );

    tgui::Label::Ptr versionLabel =
        std::make_shared<tgui::Label>();
    versionLabel->setText("Game version: v0.1");

    tgui::VerticalLayout::Ptr globalLayout =
        std::make_shared<tgui::VerticalLayout>();
    globalLayout->setSize(tgui::bindSize(gui_) * 0.9f);
    globalLayout->setPosition(tgui::bindSize(gui_) * 0.05f);

    globalLayout->add(generateTitleLayout());
    globalLayout->setRatio(0, 1.0f);

    globalLayout->add(generateMenuLayout());
    globalLayout->setRatio(1, 3.0f);

    globalLayout->add(versionLabel);
    globalLayout->setRatio(2, 0.5f);
    globalLayout->insertSpace(2, 0.1f);

    gui_.add( globalLayout );
}


void MainMenu::handleEvents()
{
    sf::Event event;
    std::unique_ptr< Level > level = nullptr;

    if( window_.pollEvent( event ) ){
        if( event.type == sf::Event::Closed ){
            requestStateExit();
        }else if( event.type == sf::Event::KeyPressed ){
            if( event.key.code == sf::Keyboard::Escape ){
                requestStateExit();
            }
        }else{
            gui_.handleEvent( event );
        }
    }
}


void MainMenu::update( unsigned int ms )
{
    (void)( ms );
}


void MainMenu::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    (void)( target );
    (void)( states );
    window_.clear( sf::Color( 0xDC, 0xF1, 0xF1, 0xFF ) );

    gui_.draw();
}


void MainMenu::pause()
{
}

void MainMenu::resume()
{
    window_.setMouseCursorVisible( true );
}


/***
 * 4. Auxiliar initialization methods
 ***/

tgui::VerticalLayout::Ptr MainMenu::generateTitleLayout() const
{
    tgui::Label::Ptr originalTitleLabel =
        std::make_shared<tgui::Label>();
    originalTitleLabel->setText("El Juego De los Bocadillos");
    originalTitleLabel->setTextSize(50);
    originalTitleLabel->setTextStyle(sf::Text::Style::Bold);

    tgui::Label::Ptr titleLabel =
        std::make_shared<tgui::Label>();
    titleLabel->setText("The Sandwiches Game");
    titleLabel->setTextSize(40);
    titleLabel->setTextStyle(sf::Text::Style::Bold);

    tgui::VerticalLayout::Ptr layout =
        std::make_shared<tgui::VerticalLayout>();
    layout->add(originalTitleLabel);
    layout->add(titleLabel);
    return layout;
}


tgui::VerticalLayout::Ptr MainMenu::generateMenuLayout() const
{
    tgui::VerticalLayout::Ptr menuLayout = std::make_shared<tgui::VerticalLayout>();

    const std::vector< std::string > buttonTexts =
    {
        "Play campaign",
        "Play survival",
        "Credits",
        "Exit game"
    };
    const std::vector< std::function<void()> > callbacks =
    {
        [this](){
            std::unique_ptr< Level > level = std::unique_ptr< Level >(
                       new CampaignLevel( window_, &soundManager_, 0 ) );
            switchState( *level );
        },
        [this](){
            std::unique_ptr< Level > level = std::unique_ptr< Level >(
                       new SurvivalLevel( window_, &soundManager_, 0 ) );
            switchState( *level );
        },
        [this](){
            std::unique_ptr< CreditsScreen > creditsScreen = std::unique_ptr< CreditsScreen >(
                       new CreditsScreen( window_ ) );
            switchState( *creditsScreen );
        },
        [this](){
            requestStateExit();
        }
    };

    // Create buttons.
    unsigned int buttonCallbackId = 0;
    for( const std::string& buttonText : buttonTexts ){
        tgui::Button::Ptr button = std::make_shared<tgui::Button>();

        button->setText( buttonText );

        button->connect( "pressed", callbacks[buttonCallbackId] );
        buttonCallbackId++;

        menuLayout->add( button );
        menuLayout->insertSpace(30, 0.5f);
    }

    // Create volume panel.
    jdb::VolumeControlPanel::Ptr volumePanel = std::make_shared<jdb::VolumeControlPanel>();
    menuLayout->add( volumePanel );

    return menuLayout;
}

} // namespace jdb
