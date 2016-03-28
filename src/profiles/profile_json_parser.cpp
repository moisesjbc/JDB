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

#include <profiles/profile_json_parser.hpp>
#include <fstream>

namespace jdb {

/***
 * Writting to JSON
 ***/

json jdb::ProfileJSONParser::writeToJSON(jdb::Profile &profile)
{
    json profileJSON;

    profileJSON["name"] = profile.name();
    profileJSON["survival_record_score"] = profile.survivalRecordScore();
    profileJSON["campaign_record_scores"] = {};
    for(unsigned int levelIndex=0; levelIndex<profile.nextCampaignLevel(); levelIndex++){
        profileJSON["campaign_record_scores"].push_back(profile.campaignLevelRecordScore(levelIndex));
    }

    const std::map<jdb::GameDifficulty, std::string> gameDifficultyStrings =
    {
        {jdb::GameDifficulty::EASY, "easy"},
        {jdb::GameDifficulty::NORMAL, "normal"},
        {jdb::GameDifficulty::HARD, "hard"}
    };
    profileJSON["game_difficulty"] =
        gameDifficultyStrings.at(profile.gameDifficulty());

    return profileJSON;
}


void ProfileJSONParser::writeToJSON(Profile &profile, std::string filePath)
{
    json profileJSON = writeToJSON(profile);

    std::ofstream file(filePath.c_str());
    file << profileJSON;
    file.close();
}


/***
 * Reading from JSON
 ***/

Profile ProfileJSONParser::readFromJSON(json profileJSON)
{
    std::string profileName = profileJSON["name"];
    Profile profile(profileName);

    profile.updateSurvivalRecordScore(profileJSON["survival_record_score"]);
    unsigned int levelIndex = 0;
    while(profileJSON["campaign_record_scores"][levelIndex] != nullptr){
        profile.updateCampaignLevelRecordScore(levelIndex,
                                               profileJSON["campaign_record_scores"][levelIndex]);
        levelIndex++;
    }

    const std::map<std::string, jdb::GameDifficulty> gameDifficultyStrings =
    {
        {"easy", jdb::GameDifficulty::EASY},
        {"normal", jdb::GameDifficulty::NORMAL},
        {"hard", jdb::GameDifficulty::HARD}
    };
    profile.updateGameDifficulty(gameDifficultyStrings.at(profileJSON["game_difficulty"]));

    return profile;
}


Profile ProfileJSONParser::readFromJSON(std::string filePath)
{
    json profileJSON;

    std::ifstream file(filePath.c_str());
    file >> profileJSON;
    file.close();

    return readFromJSON(profileJSON);
}

} // namespace jdb
