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

#include "texture_picture.hpp"

namespace jdb {

/***
 * 1. Construction
 ***/

TexturePicture::Ptr TexturePicture::create(const std::string &filename, bool fullyClickable)
{
    return std::static_pointer_cast<TexturePicture>( tgui::Picture::create( filename, fullyClickable ) );
}


/***
 * 2. Setters
 ***/

void TexturePicture::setTexture(tgui::Texture texture)
{
    texture.setPosition( m_texture.getPosition() );
    m_texture = texture;
}



} // namespace jdb
