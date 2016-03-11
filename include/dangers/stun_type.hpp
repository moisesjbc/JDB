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

#ifndef STUN_TYPE_HPP
#define STUN_TYPE_HPP

namespace jdb {

enum class StunType
{
    NONE = 0,
    BURN,
    FREEZING,
    ELECTROCUTION,
    BITE,
    HYPNOTISM,
    CUT
};

} // namespace jdb

#endif // STUN_TYPE_HPP
