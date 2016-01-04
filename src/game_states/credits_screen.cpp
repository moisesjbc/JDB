#include "credits_screen.hpp"
#include <boost/filesystem.hpp>

namespace jdb {

/***
 * 1. Construction
 ***/

CreditsScreen::CreditsScreen( sf::RenderWindow& window ) :
    GameState( window ),
    gui_( window )
{
}


/***
 * 2. GameState interface
 ***/

void CreditsScreen::init()
{
    gui_.setFont( DATA_DIR_PATH + "/fonts/LiberationSans-Bold.ttf" );

    tgui::VerticalLayout::Ptr layout = std::make_shared<tgui::VerticalLayout>();
    layout->setSize( tgui::bindSize(gui_) * 0.90f);
    layout->setPosition( tgui::bindSize(gui_) * 0.05f );

    tgui::TextBox::Ptr textBox = std::make_shared<tgui::TextBox>();
    textBox->setReadOnly( true );
    textBox->setText( generateCredits() );

    tgui::Button::Ptr exitButton = std::make_shared<tgui::Button>();
    exitButton->setText( "Exit credits" );
    exitButton->setTextSize( 20 );
    exitButton->connect( "pressed", [this](){ requestStateExit(); } );

    layout->add(textBox);
    layout->setRatio(textBox, 9);
    layout->insertSpace(1, 0.1f);

    layout->add(exitButton);
    layout->setRatio(exitButton, 1);

    gui_.add(layout);
}


void CreditsScreen::handleEvents()
{
    sf::Event event;
    if( window_.pollEvent( event ) ){
        if( event.type == sf::Event::Closed ){
            window_.close();
        }else if( event.type == sf::Event::KeyPressed ){
            if( event.key.code == sf::Keyboard::Escape ){
                requestStateExit();
            }
        }else{
            gui_.handleEvent( event );
        }
    }
}


void CreditsScreen::update(unsigned int ms)
{

}


void CreditsScreen::pause()
{

}


void CreditsScreen::resume()
{

}


/***
 * 3. Drawable interface
 ***/

void CreditsScreen::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    (void)( target );
    (void)( states );
    window_.clear( sf::Color( 0xDC, 0xF1, 0xF1, 0xFF ) );

    gui_.draw();
}


/***
 * 4. Credits generation
 ***/

std::wstring CreditsScreen::generateCredits() const
{
    std::wstring credits;

    const std::vector<std::string> filepaths =
    {
        "/LICENSE",
        "/img/LICENSE",
        "/audio/music/LICENSE",
        "/audio/player/LICENSE",
        "/audio/tools/LICENSE",
        "/fonts/LICENSE"
    };

    for( const std::string& filepath : filepaths ){
        std::wstring fileContent =
                UnicodeFileReader::readUTF8((boost::filesystem::path(DATA_DIR_PATH) / boost::filesystem::path(filepath)).string());

        credits += fileContent + L"\n";
    }

    return credits;
}


} // namespace jdb
