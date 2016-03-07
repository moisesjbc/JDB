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

#include <dangers/danger_data_parser.hpp>
#include <fstream>
#include <paths.hpp>
#include <m2g/drawables/tileset.hpp>

namespace jdb {

PlayerActionResponse DangerDataParser::ParsePlayerActionResponse(json jsonObject) const
{
    PlayerActionResponse playerActionResponse(
        jsonObject["conditions"]["player_action"],
        jsonObject["conditions"]["danger_min_hp"],
        jsonObject["conditions"]["danger_max_hp"],
        ParseDangerMutation(jsonObject["danger_mutation"])
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


StateTimeTransition DangerDataParser::ParseStateTimeTransition(json jsonObject, m2g::GraphicsLibrary& graphicsLibrary) const
{
    StateTimeTransition stateTimeTransition(
        jsonObject["min_timeout"],
        jsonObject["max_timeout"],
        ParseDangerMutation(jsonObject["danger_mutation"])
    );

    return stateTimeTransition;
}


StateDistanceTransition DangerDataParser::ParseStateDistanceTransition(json jsonObject) const
{
    StateDistanceTransition stateDistanceTransition(
        jsonObject["distance"],
        ParseDangerMutation(jsonObject["danger_mutation"])
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
    }else if( stunTypeStr == "cut" ){
        stunType = StunType::CUT;
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
                new StateTimeTransition(ParseStateTimeTransition(rawDangerStateJSON["state_time_transition"], graphicsLibrary))
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
        dangerState.appearanceAnimationData = nullptr;
    }

    if(rawDangerStateJSON["stun"] != nullptr){
        ParseDangerStateStun(rawDangerStateJSON["stun"], dangerState.stunType, dangerState.stunnedTools);
    }

    return dangerState;
}


DangerDataPtr DangerDataParser::ParseDangerData(json jsonObject, const std::vector< DangerDataPtr >& dangersDataVector, m2g::GraphicsLibrary &dangerGraphics) const
{
    // Get the danger's animation data.
    std::string dangerID = jsonObject["name"];
    m2g::AnimationDataList animDataList = dangerGraphics.getAnimationDataByPrefix( dangerID );
    std::vector< m2g::AnimationDataPtr > animationData =
            std::vector< m2g::AnimationDataPtr >{
                std::make_move_iterator(std::begin(animDataList)),
                std::make_move_iterator(std::end(animDataList)) };

    std::shared_ptr<DangerData> dangerData(new DangerData(dangerID, std::move(animationData), dangersDataVector));

    // Get the danger's general info.
    dangerData->initialState = jsonObject["initial_state"];
    dangerData->initialHp = jsonObject["initial_hp"];
    dangerData->damageFactor = jsonObject["damage_factor"];

    // Get the danger's base line.
    dangerData->baseLine = ParseBaseLine(jsonObject["base_line"]);

    // Get the damage's states.
    for( json dangerStateJSON : jsonObject["states"] ){
        dangerData->states.push_back( ParseDangerState(dangerStateJSON, dangerGraphics) );
    }

    return dangerData;
}


void DangerDataParser::LoadDangersDataByName(const std::string& configFilepath, const std::vector<std::string>& dangersIDs, m2g::GraphicsLibrary dangersGraphics, std::vector<DangerDataPtr>& dangersDataVector) const
{
    json jsonObject;

    std::ifstream file(configFilepath.c_str());
    file >> jsonObject;
    file.close();

    dangersDataVector.clear();
    for(json dangerJsonObject : jsonObject["dangers"]){
        if(std::find(dangersIDs.begin(), dangersIDs.end(), dangerJsonObject["name"]) != dangersIDs.end()){
            dangersDataVector.push_back(std::move(ParseDangerData(dangerJsonObject, dangersDataVector, dangersGraphics)));
        }
    }
}


DangerInfo DangerDataParser::ParseDangerInfo(const std::string &dangerMachineName, json jsonObject) const
{
    DangerInfo dangerInfo;

    dangerInfo.name = jsonObject["name"];
    dangerInfo.description = jsonObject["description"];
    dangerInfo.removalInstructions = jsonObject["removal_instructions"];

    dangerInfo.texturePath =
            DATA_DIR_PATH +
            "/img/dangers/" +
            dangerMachineName +
            "_01.png";

    return dangerInfo;
}


std::vector<DangerInfo> DangerDataParser::LoadDangersInfoByName(const std::string& configFilepath, const std::vector<std::string>& dangersIDs) const
{
    json jsonObject;

    std::ifstream file(configFilepath.c_str());
    file >> jsonObject;
    file.close();

    std::vector<DangerInfo> dangersInfo;
    for(json dangerJsonObject : jsonObject["dangers"]){
        if(std::find(dangersIDs.begin(), dangersIDs.end(), dangerJsonObject["name"]) != dangersIDs.end()){
            dangersInfo.push_back(ParseDangerInfo(dangerJsonObject["name"], dangerJsonObject["info"]));
        }
    }

    return dangersInfo;
}

#define ASSIGN_JSON_ATTRIBUTE(varType, varName, json, defaultVarValue) \
    varType varName = defaultVarValue; \
    if(json != nullptr){ \
        varName = json; \
    }


DangerMutation DangerDataParser::ParseDangerMutation(json jsonObject) const
{
    int dangerHpVariation = 0;
    if(jsonObject["danger_hp_variation"] != nullptr){
        if(jsonObject["danger_hp_variation"] == "all"){
            dangerHpVariation = HP_ALL;
        }else{
            dangerHpVariation = jsonObject["danger_hp_variation"];
        }
    }
    ASSIGN_JSON_ATTRIBUTE(unsigned int, newDangerState, jsonObject["new_danger_state"], 0)
    ASSIGN_JSON_ATTRIBUTE(DangerID, newDanger, jsonObject["new_danger"]["id"], "")
    ASSIGN_JSON_ATTRIBUTE(std::string, newDangerAppearanceAnimation, jsonObject["new_danger"]["appearance_animation"], "")
    ASSIGN_JSON_ATTRIBUTE(int, playerHpVariation, jsonObject["player_hp_variation"], 0)
    ASSIGN_JSON_ATTRIBUTE(int, playerScoreVariation, jsonObject["player_score_variation"], 0)

    return DangerMutation(
       dangerHpVariation,
       newDangerState,
       newDanger,
       newDangerAppearanceAnimation,
       playerHpVariation,
       playerScoreVariation
    );
}

} // namespace jdb
