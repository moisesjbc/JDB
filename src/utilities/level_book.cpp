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
#include <paths.hpp>

namespace jdb {

/***
 * 1. Construction
 ***/

LevelBook::LevelBook() :
    currentPageIndex_(0)
{
    const sf::Vector2f LEVEL_BOOK_PADDING = { 5.0f, 5.0f };
    const sf::Vector2f LEVEL_BOOK_SIZE = { 600.0f, 400.0f };
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

    picture_ = TexturePicture::create( DATA_DIR_PATH + "/img/sandwiches/sandwich_01.png" );
    picture_->setPosition( 0.0f,
                           tgui::bindBottom( textBox_ ) );
    Container::add( picture_ );

    continueButton_ = tgui::Button::create();
    continueButton_->setPosition( 5.0f, tgui::bindBottom( picture_ ) );
    continueButton_->setText( "Continue" );
    continueButton_->setSize( BUTTON_SIZE.x, BUTTON_SIZE.y );
    Container::add( continueButton_ );

    m_callback.widgetType = "LevelBook";
    addSignal<sf::String>("BookClosed");
    continueButton_->connect( "pressed", std::bind( &LevelBook::setNextPage, this ) );

    this->connect( "BookClosed", std::bind( &LevelBook::hide, this ) );
}


LevelBook::Ptr LevelBook::create()
{
    auto book = std::make_shared<LevelBook>();

    return book;
}


/***
 * 3. Pages management
 ***/

void LevelBook::addPage( const sf::String &text,
                         std::unique_ptr<tgui::Texture> texture)
{
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
    if( newPage.texture != nullptr ){
        picture_->setTexture( *( newPage.texture ) );
    }
    textBox_->setSize( textBox_->getSize().x,
                       getSize().y - picture_->getSize().y - continueButton_->getSize().y );
}


void LevelBook::setNextPage()
{
    if( pages_.size() &&
            ( currentPageIndex_ < ( pages_.size() - 1 ) ) ){
        setPage( currentPageIndex_ + 1 );
    }else{
        sendSignal( "bookClosed" );
    }
}


void LevelBook::setSize(const tgui::Layout1d &width,
                        const tgui::Layout1d &height)
{
    const sf::Vector2f SCALE_FACTORS =
    {
        width.getValue() / getSize().x,
        height.getValue() / getSize().y
    };
    for( Widget::Ptr& widget : m_widgets ){
        widget->scale( SCALE_FACTORS );
    }

    Panel::setSize( width, height );
}

} // namespace jdb

