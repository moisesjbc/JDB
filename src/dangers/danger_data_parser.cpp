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
#include <fstream>

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
        std::set<ToolType>& stunnedTools) const
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


DangerState DangerDataParser::ParseDangerState(json rawDangerStateJSON, m2g::GraphicsLibrary& graphicsLibrary) const
{
    DangerState dangerState;

    dangerState.animationState = rawDangerStateJSON["animation_state"];

    for(json playerActionJSONObject : rawDangerStateJSON["player_action_responses"]){
        dangerState.playerActionResponses.push_back(ParsePlayerActionResponse(playerActionJSONObject));
    }

    if(rawDangerStateJSON["state_time_transition"] != nullptr){
        dangerState.stateTimeTransition =
            std::unique_ptr<StateTimeTransition>(
                new StateTimeTransition(ParseStateTimeTransition(rawDangerStateJSON["state_time_transition"]))
            );
    }else{
        dangerState.stateTimeTransition = nullptr;
    }

    if(rawDangerStateJSON["state_distance_transition"] != nullptr){
        dangerState.stateDistanceTransition =
            std::unique_ptr<StateDistanceTransition>(
                new StateDistanceTransition(ParseStateDistanceTransition(rawDangerStateJSON["state_distance_transition"]))
            );
    }else{
        dangerState.stateDistanceTransition = nullptr;
    }

    if(rawDangerStateJSON["random_danger_on_animation_state_end"] != nullptr){
        dangerState.randomDangerOnAnimationStateEnd = true;

        dangerState.appearanceAnimationData =
                graphicsLibrary.getAnimationDataByName(
                    rawDangerStateJSON["random_danger_on_animation_state_end"]["appearance_animation"] );
    }else{
        dangerState.randomDangerOnAnimationStateEnd = false;
    }

    ParseDangerStateStun(rawDangerStateJSON["stun"], dangerState.stunType, dangerState.stunnedTools);

    return dangerState;
}


DangerDataPtr DangerDataParser::ParseDangerData(json jsonObject, const std::vector< DangerDataPtr >& dangersDataVector, m2g::GraphicsLibrary &dangerGraphics) const
{
    // Get the danger's animation data.
    std::string dangerName = jsonObject["name"];
    m2g::AnimationDataList animDataList = dangerGraphics.getAnimationDataByPrefix( dangerName );
    std::vector< m2g::AnimationDataPtr > animationData =
            std::vector< m2g::AnimationDataPtr >{
                std::make_move_iterator(std::begin(animDataList)),
                std::make_move_iterator(std::end(animDataList)) };

    std::shared_ptr<DangerData> dangerData(new DangerData(std::move(animationData), dangersDataVector));

    // Get the danger's general info.
    dangerData->initialState = jsonObject["initial_state"];
    dangerData->initialHp = jsonObject["initial_hp"];
    dangerData->damageFactor = jsonObject["damage_factor"];

    // Get the danger's base line.
    dangerData->baseLine = ParseBaseLine(jsonObject["base_line"]);

    // Get the damage's states.
    for( json dangerStateJSON : jsonObject["danger_states"] ){
        dangerData->states.push_back( ParseDangerState(dangerStateJSON, dangerGraphics) );
    }

    return dangerData;
}


std::vector<DangerDataPtr> DangerDataParser::LoadLevelDangerData(const std::string& configFilepath, unsigned int levelIndex, m2g::GraphicsLibrary dangersGraphics) const
{
    json jsonObject;

    std::ifstream file(configFilepath.c_str());
    file >> jsonObject;
    file.close();

    std::vector<DangerDataPtr> dangersData;
    for(json dangerJsonObject : jsonObject["dangers"]){
        if(dangerJsonObject["first_level"] <= levelIndex){
            dangersData.push_back(std::move(ParseDangerData(dangerJsonObject, dangersData, dangersGraphics)));
        }
    }

    return dangersData;
}

} // namespace jdb
