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

#include "level_book.hpp"

namespace jdb {

/***
 * 1. Construction
 ***/

LevelBook::LevelBook() :
    currentPageIndex_(0)
{
    const sf::Vector2f LEVEL_BOOK_PADDING = { 5.0f, 5.0f };
    const sf::Vector2f LEVEL_BOOK_SIZE = { 400.0f, 300.0f };
    const sf::Vector2f TEXT_BOX_SIZE =
    {
        LEVEL_BOOK_SIZE.x - LEVEL_BOOK_PADDING.x * 2.0f,
        LEVEL_BOOK_SIZE.y * 0.8f - LEVEL_BOOK_PADDING.y * 2.0f
    };
    const sf::Vector2f BUTTON_SIZE = { LEVEL_BOOK_SIZE.x - LEVEL_BOOK_PADDING.x * 2.0f,
                                       LEVEL_BOOK_SIZE.y * 0.2f - LEVEL_BOOK_PADDING.y };

    tgui::Panel::setSize( LEVEL_BOOK_SIZE.x, LEVEL_BOOK_SIZE.y );

    textBox_ = tgui::TextBox::create();
    textBox_->setPosition( LEVEL_BOOK_PADDING.x, LEVEL_BOOK_PADDING.y );
    textBox_->setReadOnly( true );
    textBox_->setSize( TEXT_BOX_SIZE.x, TEXT_BOX_SIZE.y );
    Container::add( textBox_ );

    continueButton_ = tgui::Button::create();
    continueButton_->setPosition( 5.0f, tgui::bindBottom( textBox_ ) + 5.0f );
    continueButton_->setText( "Continue" );
    continueButton_->setSize( BUTTON_SIZE.x, BUTTON_SIZE.y );
    Container::add( continueButton_ );

    m_callback.widgetType = "LevelBook";
    addSignal<sf::String>("BookClosed");
    continueButton_->connect( "pressed", std::bind( &LevelBook::setNextPage, this ) );
}


LevelBook::Ptr LevelBook::create()
{
    auto book = std::make_shared<LevelBook>();

    return book;
}


/***
 * 3. Pages management
 ***/

void LevelBook::addPage(const sf::String &text)
{
    pagesText_.push_back( text );
    if( pagesText_.size() == 1 ){
        setPage( 0 );
    }
}


/***
 * 4. Setters
 ***/

void LevelBook::setPage(unsigned int pageIndex)
{
    currentPageIndex_ = pageIndex;
    textBox_->setText( pagesText_.at( pageIndex ) );
}


void LevelBook::setNextPage()
{
    if( pagesText_.size() &&
            ( currentPageIndex_ < ( pagesText_.size() - 1 ) ) ){
        setPage( currentPageIndex_ + 1 );
    }else{
        sendSignal( "bookClosed" );
    }
}

} // namespace jdb

