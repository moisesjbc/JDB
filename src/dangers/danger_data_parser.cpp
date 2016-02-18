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
        jsonObject["conditions"]["player_action"],
        jsonObject["conditions"]["danger_min_hp"],
        jsonObject["conditions"]["danger_max_hp"],
        jsonObject["consequences"]["danger_hp_variation"],
        jsonObject["consequences"]["new_state"],
        jsonObject["consequences"]["new_danger"],
        jsonObject["consequences"]["player_hp_bonus"]
    );

    return playerActionResponse;
}


BaseLine DangerDataParser::ParseBaseLine(json jsonObject)
{
    BaseLine baseLine(
        jsonObject["x"],
        jsonObject["y"],
        jsonObject["width"]
    );

    return baseLine;
}


StateTimeTransition DangerDataParser::ParseStateTimeTransition(json jsonObject) const
{
    StateTimeTransition stateTimeTransition(
        jsonObject["min_timeout"],
        jsonObject["max_timeout"],
        jsonObject["new_state"]
    );

    return stateTimeTransition;
}


StateDistanceTransition DangerDataParser::ParseStateDistanceTransition(json jsonObject) const
{
    StateDistanceTransition stateDistanceTransition(
        jsonObject["distance"],
        jsonObject["new_state"]
    );

    return stateDistanceTransition;
}


void DangerDataParser::ParseDangerStateStun(
        json rawDangerStateStunJSON,
        jdb::StunType& stunType,
        std::set<ToolType>& stunnedTools)
{
    std::string stunTypeStr = rawDangerStateStunJSON["type"];
    if( stunTypeStr == "burn" ){
        stunType = StunType::BURN;
    }else if( stunTypeStr == "freezing" ){
        stunType = StunType::FREEZING;
    }else if( stunTypeStr == "electrocution" ){
        stunType = StunType::ELECTROCUTION;
    }else if( stunTypeStr == "bite" ){
        stunType = StunType::BITE;
    }else if( stunTypeStr == "hypnotism" ){
        stunType = StunType::HYPNOTISM;
    }else{
        throw std::runtime_error( "Unrecognized stun" );
    }

    for(const std::string& toolStr : rawDangerStateStunJSON["tools"]){
        if( toolStr == "hand" ){
            stunnedTools.insert( ToolType::HAND );
        }else if( toolStr == "extinguisher" ){
            stunnedTools.insert( ToolType::EXTINGUISHER );
        }else if( toolStr == "lighter" ){
            stunnedTools.insert( ToolType::LIGHTER );
        }else if( toolStr == "gavel" ){
            stunnedTools.insert( ToolType::GAVEL );
        }else{
            throw std::runtime_error( "Unrecognized stunned tool" );
        }
    }
}

} // namespace jdb
