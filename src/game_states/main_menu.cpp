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

#include <game_states/main_menu.hpp>
#include <levels/survival_level.hpp>
#include <levels/campaign_level.hpp>
#include "game_states/credits_screen.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics/Font.hpp>
#include <utilities/volume_control_panel.hpp>
#include <utilities/version_string.hpp>
#include <game_states/campaign_level_selection_menu.hpp>

namespace jdb {

/***
 * 1. Creation
 ****/

MainMenu::MainMenu( sf::RenderWindow& window, SoundManager* soundManager, Profile& playerProfile ) :
    GUIMenu(window),
    soundManager_(*soundManager),
    playerProfile_(playerProfile)
{}


/***
 * 3. GameState interface
 ***/

void MainMenu::initGUI(tgui::Gui& gui)
{
    gui.setFont( DATA_DIR_PATH + "/fonts/LiberationSans-Bold.ttf" );

    tgui::Label::Ptr versionLabel =
        std::make_shared<tgui::Label>();
    versionLabel->setText("Game version: " + VERSION_STRING);

    tgui::VerticalLayout::Ptr globalLayout =
        std::make_shared<tgui::VerticalLayout>();
    globalLayout->setSize(tgui::bindSize(gui) * 0.9f);
    globalLayout->setPosition(tgui::bindSize(gui) * 0.05f);

    globalLayout->add(generateTitleLayout());
    globalLayout->setRatio(0, 1.0f);

    globalLayout->add(generateMenuLayout());
    globalLayout->setRatio(1, 3.0f);

    globalLayout->add(versionLabel);
    globalLayout->setRatio(2, 0.5f);
    globalLayout->insertSpace(2, 0.1f);

    gui.add( globalLayout );
}


/***
 * 4. GameState interface
 ***/

void MainMenu::resume()
{
    GUIMenu::resume();
    currentSurvivalRecordScoreLabel_->setText("Current record score: " +
                                              std::to_string(playerProfile_.survivalRecordScore()));
}


/***
 * 5. Auxiliar initialization methods
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


tgui::VerticalLayout::Ptr MainMenu::generateMenuLayout()
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
            CampaignLevelSelectionMenu levelSelectionMenu(window_,
                                                          soundManager_,
                                                          CampaignLevel::nLevels(),
                                                          playerProfile_);
            switchState( levelSelectionMenu );
        },
        [this](){
            std::unique_ptr< Level > level = std::unique_ptr< Level >(
                       new SurvivalLevel( window_, &soundManager_, 0, playerProfile_ ) );
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
        if(buttonCallbackId == 1){
            currentSurvivalRecordScoreLabel_ =
                std::make_shared<tgui::Label>();
            currentSurvivalRecordScoreLabel_->setText("Current record score: " +
                                                      std::to_string(playerProfile_.survivalRecordScore()));
            button->setToolTip(currentSurvivalRecordScoreLabel_);
            tgui::ToolTip::setTimeToDisplay(sf::Time::Zero);
        }
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
