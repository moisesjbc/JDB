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
    m_callback.widgetType = "LevelBook";
    addSignal<sf::String>("BookClosed");
    this->connect( "ButtonPressed", std::bind( &LevelBook::setNextPage, this ) );
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
    setText( pagesText_.at( pageIndex ) );
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

