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
#include <array>


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



TEST(ProfileTest, nextCampaignLevelIsUpdatedCorrectly)
{
    jdb::Profile profile("profile-name");
    profile.updateCampaignLevelRecordScore(0, 15);
    EXPECT_EQ(1, profile.nextCampaignLevel());
    profile.updateCampaignLevelRecordScore(0, 15);
    EXPECT_EQ(1, profile.nextCampaignLevel());
    profile.updateCampaignLevelRecordScore(3, 15);
    EXPECT_EQ(4, profile.nextCampaignLevel());
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


TEST(ProfileTest, campaignRecordScoreIsUpdatedWithBiggerScores)
{
    jdb::Profile profile("profile-name");
    EXPECT_EQ(true, profile.updateCampaignLevelRecordScore(0, 55));
    EXPECT_EQ(55, profile.campaignLevelRecordScore(0));
}


TEST(ProfileTest, campaignRecordScoreIsNotUpdatedWithSmallerScores)
{
    jdb::Profile profile("profile-name");
    const unsigned int bestCampaignRecordScore = 55;
    const unsigned int levelIndex = 0;
    profile.updateCampaignLevelRecordScore(levelIndex, bestCampaignRecordScore);
    EXPECT_EQ(bestCampaignRecordScore, profile.campaignLevelRecordScore(levelIndex));

    EXPECT_EQ(false, profile.updateCampaignLevelRecordScore(levelIndex, 30));
    EXPECT_EQ(bestCampaignRecordScore, profile.campaignLevelRecordScore(levelIndex));

    EXPECT_EQ(false, profile.updateCampaignLevelRecordScore(levelIndex, 45));
    EXPECT_EQ(bestCampaignRecordScore, profile.campaignLevelRecordScore(levelIndex));
}


TEST(ProfileTest, campaignRecordsAreInitializedToZero)
{
    jdb::Profile profile("profile-name");
    const unsigned int lastLevelIndex = 5;
    profile.updateCampaignLevelRecordScore(lastLevelIndex, 55);

    for(unsigned int levelIndex=0; levelIndex<lastLevelIndex; levelIndex++){
        EXPECT_EQ(0, profile.campaignLevelRecordScore(levelIndex));

    }
    EXPECT_EQ(55, profile.campaignLevelRecordScore(lastLevelIndex));
}


TEST(ProfileTest, accesingAnUnsavedCampaignRecordReturnsZero)
{
    jdb::Profile profile("profile-name");
    EXPECT_EQ(0, profile.campaignLevelRecordScore(0));
}


TEST(ProfileTest, gameDifficultyDefaultsToNormal)
{
    jdb::Profile profile("profile-name");
    EXPECT_EQ(jdb::GameDifficulty::NORMAL, profile.gameDifficulty());
}


TEST(ProfileTest, updatingGameDifficultyWorks)
{
    jdb::Profile profile("profile-name");

    std::array<jdb::GameDifficulty, 3> gameDifficulties =
    {
        jdb::GameDifficulty::EASY,
        jdb::GameDifficulty::NORMAL,
        jdb::GameDifficulty::HARD
    };

    for(jdb::GameDifficulty newGameDifficulty : gameDifficulties){
        EXPECT_EQ(true, profile.updateGameDifficulty(newGameDifficulty));
        EXPECT_EQ(newGameDifficulty, profile.gameDifficulty());
    }
}


TEST(ProfileTest, updateGameDifficultyMethodReturnsFalseIfGameDifficultyDoesNotChange)
{
    jdb::Profile profile("profile-name");

    // Game difficulty defaults to normal, so this should return false.
    EXPECT_EQ(false, profile.updateGameDifficulty(jdb::GameDifficulty::NORMAL));

    // Game difficulty changed from normal to hard, this should flag the change.
    EXPECT_EQ(true, profile.updateGameDifficulty(jdb::GameDifficulty::HARD));

    // Game difficulty already set to hard, so this should not flag any changes.
    EXPECT_EQ(false, profile.updateGameDifficulty(jdb::GameDifficulty::HARD));
}
