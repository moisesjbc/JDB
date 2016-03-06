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

#ifndef DANGER_DATA_PARSER_HPP
#define DANGER_DATA_PARSER_HPP

#include <dangers/danger_info.hpp>
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
        StateTimeTransition ParseStateTimeTransition(json jsonObject, m2g::GraphicsLibrary& graphicsLibrary) const;
        StateDistanceTransition ParseStateDistanceTransition(json jsonObject) const;
        jdb::StunType stuntype;
        std::set<ToolType> stunnedTools;
        void ParseDangerStateStun(
                json rawDangerStateStunJSON,
                jdb::StunType& stunType,
                std::set<ToolType>& stunnedTools) const;
        DangerState ParseDangerState(json rawDangerStateJSON, m2g::GraphicsLibrary& graphicsLibrary) const;
        DangerDataPtr ParseDangerData(json rawDangerStateJSON, const std::vector< DangerDataPtr >& dangersDataVector, m2g::GraphicsLibrary& dangerGraphics) const;
        void LoadDangersDataByName(const std::string& configFilepath, const std::vector<std::string>& dangersIDs, m2g::GraphicsLibrary dangersGraphics, std::vector<DangerDataPtr>& dangersDataVector) const;
        DangerInfo ParseDangerInfo(const std::string& dangerMachineName, json jsonObject) const;
        std::vector<DangerInfo> LoadDangersInfoByName(const std::string& configFilepath, const std::vector<std::string>& dangersIDs) const;
        DangerMutation ParseDangerMutation(json jsonObject) const;
};

} // namespace jdb

#endif // DANGER_DATA_PARSER_HPP
