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

#include <profiles/profile.hpp>
#include <gtest/gtest.h>
#include <vector>

TEST(ProfileTest, nameIsInitializedInConstructor)
{
    std::vector<std::string> names =
        {"foo", "bar", "name with spaces"};

    for(const std::string& name : names){
        jdb::Profile profile(name);
        EXPECT_EQ(name, profile.name());
    }
}


TEST(ProfileTest, profileNamesWithLessThanThreeCharactersAreNotAllowed)
{
    std::vector<std::string> names =
        {"", "a", "ab"};

    for(const std::string& name : names){
        try{
            jdb::Profile profile(name);
            FAIL() << "Expected std::invalid_argument bacause of small name";
        }catch(std::invalid_argument& e){
            EXPECT_EQ("Profile name with less than 3 characters not allowed", std::string(e.what()));
        }catch(...){
            FAIL() << "Expected std::invalid_argument bacause of small name";
        }
    }
}


TEST(ProfileTest, nextCampaignLevelIsInitializedToZero)
{
    jdb::Profile profile("profile-name");
    EXPECT_EQ(0, profile.nextCampaignLevel());
}



TEST(ProfileTest, nextCampaignLevelSetterWorks)
{
    jdb::Profile profile("profile-name");
    profile.setNextCampaignLevel(66);
    EXPECT_EQ(66, profile.nextCampaignLevel());
}


TEST(ProfileTest, survivalRecordScoreIsUpdatedWithBiggerScores)
{
    jdb::Profile profile("profile-name");
    EXPECT_EQ(0, profile.survivalRecordScore());
    EXPECT_EQ(true, profile.updateSurvivalRecordScore(55));
    EXPECT_EQ(55, profile.survivalRecordScore());
    EXPECT_EQ(true, profile.updateSurvivalRecordScore(70));
    EXPECT_EQ(70, profile.survivalRecordScore());
}


TEST(ProfileTest, survivalRecordScoreIsNotUpdatedWithSmallerScores)
{
    jdb::Profile profile("profile-name");
    unsigned int bestSurvivalRecordScore = 55;
    profile.updateSurvivalRecordScore(bestSurvivalRecordScore);
    EXPECT_EQ(bestSurvivalRecordScore, profile.survivalRecordScore());

    EXPECT_EQ(false, profile.updateSurvivalRecordScore(30));
    EXPECT_EQ(bestSurvivalRecordScore, profile.survivalRecordScore());

    EXPECT_EQ(false, profile.updateSurvivalRecordScore(45));
    EXPECT_EQ(bestSurvivalRecordScore, profile.survivalRecordScore());
}
