/***
    Copyright 2013 - 2019 Moises J. Bonilla Caraballo (moisesjbc)

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

#include <utilities/level_book.hpp>
#include <TGUI/Widgets/VerticalLayout.hpp>
#include <paths.hpp>

namespace jdb {

/***
 * 1. Construction
 ***/

LevelBook::LevelBook() :
    currentPageIndex_(0)
{
    const sf::Vector2f LEVEL_BOOK_SIZE = { 600.0f, 400.0f };

    getRenderer()->setBackgroundColor( sf::Color::White );

    tgui::VerticalLayout::Ptr layout = std::make_shared<tgui::VerticalLayout>();

    textBox_ = std::make_shared<tgui::TextBox>();
    textBox_->setReadOnly( true );
    layout->add( textBox_ );
    layout->setRatio( textBox_, 2.0f );

    picture_ = TexturePicture::create( DATA_DIR_PATH + "/img/sandwiches/sandwich_01.png" );
    layout->add( picture_ );

    continueButton_ = std::make_shared<tgui::Button>();
    continueButton_->setText( "Continue" );
    layout->add( continueButton_ );
    layout->setRatio( continueButton_, 0.5f );

    add(layout);

    continueButton_->connect( "pressed", &LevelBook::setNextPage, this );
    connect( "BookClosed", &LevelBook::setVisible, this, false );
    setSize( LEVEL_BOOK_SIZE.x, LEVEL_BOOK_SIZE.y );
}


/***
 * 3. Pages management
 ***/

void LevelBook::addPage( const sf::String &text,
                         std::unique_ptr<tgui::Sprite> texture)
{
    if(texture != nullptr){
        texture->setVisibleRect(sf::FloatRect(0.0f, 0.0f, picture_->getSize().x, picture_->getSize().y));
    }
    pages_.emplace_back( text, std::move( texture ) );
    if( pages_.size() == 1 ){
        setPage( 0 );
    }
}


/***
 * 4. Setters
 ***/

void LevelBook::setPage(unsigned int pageIndex)
{
    currentPageIndex_ = pageIndex;

    BookPage& newPage = pages_.at( pageIndex );

    textBox_->setText( newPage.text );

    // A trick for setting the caret at the beginning of the text :-)
    /*sf::Event::KeyEvent keyEvent;
    keyEvent.code = sf::Keyboard::PageUp;
    while( textBox_->getScrollbar()->getValue() > 0 ){
        textBox_->keyPressed( keyEvent );
    }*/

    if( newPage.sprite != nullptr ){
        picture_->setTexture( newPage.sprite->getTexture() );
    }
}


void LevelBook::setNextPage()
{
    if( pages_.size() &&
            ( currentPageIndex_ < ( pages_.size() - 1 ) ) ){
        setPage( currentPageIndex_ + 1 );
    }else{
        onBookClosed.emit(this, true);
    }
}


/***
 * 5. Signals
 ***/

tgui::Signal& LevelBook::getSignal(std::string signalName)
{
    if (signalName == tgui::toLower(onBookClosed.getName())) {
        return onBookClosed;
    }

    return tgui::Panel::getSignal(signalName);
}

} // namespace jdb

