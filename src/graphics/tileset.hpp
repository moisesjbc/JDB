/***
    Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)

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

#ifndef TILESET_HPP
#define TILESET_HPP

#include "drawable.hpp"
#include <memory>

namespace jdb {

struct TilesetData {

};

class Tileset : public Drawable
{
    private:
        static GLuint vao;
        std::shared_ptr< TilesetData > tilesetData;

    public:
        /***
         * 1. Initialization
         ***/
        Tileset();
        void setTileSetData( const std::shared_ptr< TilesetData >& tilesetData );
};


} // Namespace jdb

#endif // TILESET_HPP
