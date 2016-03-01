#include <game_states/game_over_screen.hpp>
#include <paths.hpp>

namespace jdb {

/***
 * 1. Construction
 ***/

GameOverScreen::GameOverScreen(sf::RenderWindow &window, const GameState& parentGameState) :
    GameState(window),
    gui_(window),
    parentGameState_(parentGameState)
{}


/***
 * 2. GameState interface
 ***/

void GameOverScreen::init()
{
    gui_.setFont( DATA_DIR_PATH + "/fonts/LiberationSans-Bold.ttf" );

    tgui::Panel::Ptr panel =
        std::make_shared<tgui::Panel>();
    panel->setBackgroundColor(sf::Color::White);
    panel->setSize(tgui::bindSize(gui_) * 0.3f);
    panel->setPosition(tgui::bindSize(gui_) * 0.35f);

    tgui::Label::Ptr gameOverLabel =
        std::make_shared<tgui::Label>();
    gameOverLabel->setText("Game Over");
    gameOverLabel->setTextSize(30);

    tgui::Button::Ptr restartButton =
        std::make_shared<tgui::Button>();
    restartButton->setText("Restart level");
    restartButton->setTextSize(20);
    restartButton->connect("pressed", [this](){ requestStateExit(); });

    tgui::Button::Ptr returnButton =
        std::make_shared<tgui::Button>();
    returnButton->setText("Return to main menu");
    returnButton->setTextSize(20);
    returnButton->connect("pressed", [this](){ requestStateExit(-1); });

    tgui::VerticalLayout::Ptr layout =
        std::make_shared<tgui::VerticalLayout>();
    layout->setSize(tgui::bindSize(panel));
    layout->add(gameOverLabel);
    layout->setRatio(gameOverLabel, 3);
    layout->insertSpace(999, 1.0f);
    layout->add(restartButton);
    layout->setRatio(restartButton, 1);
    layout->insertSpace(999, 1.0f);
    layout->add(returnButton);
    layout->setRatio(returnButton, 1);
    layout->insertSpace(999, 1.0f);

    panel->add(layout);
    gui_.add(panel);

    window_.setMouseCursorVisible( true );
}


void GameOverScreen::handleEvents()
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


void GameOverScreen::update(unsigned int ms)
{
    (void)(ms);
}


void GameOverScreen::pause()
{

}


void GameOverScreen::resume()
{
    window_.setMouseCursorVisible( true );
}


/***
 * 3. Drawable interface
 ***/

void GameOverScreen::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(parentGameState_, states);
    gui_.draw();
}

} // namespace jdb
