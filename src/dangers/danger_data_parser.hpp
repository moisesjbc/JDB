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

#ifndef DANGER_DATA_PARSER_HPP
#define DANGER_DATA_PARSER_HPP

#include <dangers/danger_data.hpp>
#include <utilities/base_line.hpp>
#include <utilities/json.hpp>
using json = nlohmann::json;

namespace jdb {

class DangerDataParser
{
    public:
        PlayerActionResponse ParsePlayerActionResponse(json jsonObject) const;
        static BaseLine ParseBaseLine(json jsonObject);
        StateTimeTransition ParseStateTimeTransition(json jsonObject) const;
        StateDistanceTransition ParseStateDistanceTransition(json jsonObject) const;
        jdb::StunType stuntype;
        std::set<ToolType> stunnedTools;
        void ParseDangerStateStun(
                json rawDangerStateStunJSON,
                jdb::StunType& stunType,
                std::set<ToolType>& stunnedTools);
        DangerState ParseDangerState(json rawDangerStateJSON, m2g::GraphicsLibrary& graphicsLibrary);
};

} // namespace jdb

#endif // DANGER_DATA_PARSER_HPP
