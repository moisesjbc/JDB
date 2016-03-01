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

#include <utilities/texture_picture.hpp>

namespace jdb {

/***
 * 1. Construction
 ***/

TexturePicture::Ptr TexturePicture::create(const std::string &filename, bool fullyClickable)
{
    return std::static_pointer_cast<TexturePicture>( std::make_shared<Picture>( filename, fullyClickable ) );
}


/***
 * 2. Setters
 ***/

void TexturePicture::setTexture(tgui::Texture texture)
{
    texture.setPosition( m_texture.getPosition() );
    m_texture = texture;
    setSize( m_texture.getSize() );
}



} // namespace jdb
