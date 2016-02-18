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

#include <dangers/danger_data_parser.hpp>

namespace jdb {

PlayerActionResponse DangerDataParser::ParsePlayerActionResponse(json jsonObject) const
{
    PlayerActionResponse playerActionResponse(
        jsonObject["state_transition"]["conditions"]["player_action"],
        jsonObject["state_transition"]["conditions"]["danger_min_hp"],
        jsonObject["state_transition"]["conditions"]["danger_max_hp"],
        jsonObject["state_transition"]["consequences"]["danger_hp_variation"],
        jsonObject["state_transition"]["consequences"]["new_state"],
        jsonObject["state_transition"]["consequences"]["new_danger"],
        jsonObject["state_transition"]["consequences"]["player_hp_bonus"]
    );

    return playerActionResponse;
}


BaseLine DangerDataParser::ParseBaseLine(json jsonObject) const
{
    BaseLine baseLine(
        jsonObject["x"],
        jsonObject["y"],
        jsonObject["width"]
    );

    return baseLine;
}

} // namespace jdb
