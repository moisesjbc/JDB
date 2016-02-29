#include "credits_screen.hpp"
#include <boost/filesystem.hpp>

namespace jdb {

/***
 * 1. Construction
 ***/

CreditsScreen::CreditsScreen( sf::RenderWindow& window ) :
    GUIMenu( window )
{}


/***
 * 2. GameState interface
 ***/

void CreditsScreen::initGUI(tgui::Gui &gui)
{
    gui.setFont( DATA_DIR_PATH + "/fonts/LiberationSans-Bold.ttf" );

    tgui::VerticalLayout::Ptr layout = std::make_shared<tgui::VerticalLayout>();
    layout->setSize( tgui::bindSize(gui) * 0.90f);
    layout->setPosition( tgui::bindSize(gui) * 0.05f );

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

    gui.add(layout);
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
