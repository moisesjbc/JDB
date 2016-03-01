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

#include <dangers/dangers_counter.hpp>
#include <stdexcept>
#include <gtest/gtest.h>

TEST(DangersCounterTest, nDangersIsInitializedInConstructor) {
    std::vector<int> nDangersVector = {5, 1, 15};
    for( int nDangers : nDangersVector ){
        jdb::DangersCounter dangersFactoryA(nDangers, {{"danger", 1.0f}});
        EXPECT_EQ(nDangers, dangersFactoryA.nDangers());
    }
}


TEST(DangersCounterTest, CantCreateEmptyDangersCounter){
    try{
        jdb::DangersCounter emptyDangersCounter(0, {{"danger", 1.0f}});
        FAIL() << "Expected std::out_of_range";
    }catch(std::out_of_range const&){
        SUCCEED();
    }catch(...){
        FAIL() << "Expected std::out_of_range";
    }
}


TEST(DangersCounterTest, CantCreateMoreSpecificDangersThanTotalSum){
    try{
        jdb::DangersCounter dangersCounter(1, {{"dangerA", 1.0f}, {"dangerB", 1.0f}});
        FAIL() << "Expected std::runtime_error because of having more specific dangers than total sum of dangers";
    }catch(std::runtime_error const& e){
        EXPECT_EQ(0, strcmp(e.what(), "Can't create a DangersCounter with more specific dangers than total sum of dangers"));
    }catch(...){
        FAIL() << "Expected std::out_of_range";
    }
}


TEST(DangersCounterTest, AnUniqueDangerOccupiesTheFullCounter) {
    unsigned int nDangers = 15;
    jdb::DangersCounter dangersFactoryA(nDangers, {{"danger", 1}});
    EXPECT_EQ(nDangers, dangersFactoryA.nDangers("danger"));
}


TEST(DangersCounterTest, DangersTotalIsDistributedBetweenSpecificDangersWithEqualRatios) {
    unsigned int nDangers = 15;
    jdb::DangersCounter dangersFactory(nDangers, {{"dangerA", 1}, {"dangerB", 1}, {"dangerC", 1}});
    EXPECT_EQ(nDangers / 3, dangersFactory.nDangers("dangerA"));
    EXPECT_EQ(nDangers / 3, dangersFactory.nDangers("dangerB"));
    EXPECT_EQ(nDangers / 3, dangersFactory.nDangers("dangerC"));
}


TEST(DangersCounterTest, DangersTotalIsDistributedBetweenSpecificDangersWithDifferentRatios) {
    jdb::DangersCounter dangersFactory(12, {{"dangerA", 1}, {"dangerB", 2}, {"dangerC", 3}});
    EXPECT_EQ(2, dangersFactory.nDangers("dangerA"));
    EXPECT_EQ(4, dangersFactory.nDangers("dangerB"));
    EXPECT_EQ(6, dangersFactory.nDangers("dangerC"));
}


TEST(DangersCounterTest, DangersTotalIsDistributedBetweenSpecificDangersWithDifferentRatios_DivisionNotExact) {
    jdb::DangersCounter dangersFactory(14, {{"dangerA", 1}, {"dangerB", 2}, {"dangerC", 3}});
    EXPECT_EQ(2, dangersFactory.nDangers("dangerA"));
    EXPECT_EQ(4, dangersFactory.nDangers("dangerB"));
    EXPECT_EQ(8, dangersFactory.nDangers("dangerC"));
}


TEST(DangersCounterTest, DangersCountersCanBeDecreased) {
    jdb::DangersCounter dangersFactory(12, {{"dangerA", 1}, {"dangerB", 2}, {"dangerC", 3}});
    EXPECT_EQ(2, dangersFactory.nDangers("dangerA"));
    dangersFactory.decreaseDangerCounter("dangerA");
    EXPECT_EQ(1, dangersFactory.nDangers("dangerA"));

    EXPECT_EQ(4, dangersFactory.nDangers("dangerB"));
    dangersFactory.decreaseDangerCounter("dangerB");
    EXPECT_EQ(3, dangersFactory.nDangers("dangerB"));

    EXPECT_EQ(6, dangersFactory.nDangers("dangerC"));
    dangersFactory.decreaseDangerCounter("dangerC");
    EXPECT_EQ(5, dangersFactory.nDangers("dangerC"));
}


TEST(DangersCounterTest, CantCreateADangersCounterWithEmptyIDsSet) {
    try{
        jdb::DangersCounter dangersFactory(15, {});
        FAIL() << "Expected an out_of_range exception because of empty dangers IDs set";
    }catch(std::out_of_range& e){
        EXPECT_EQ(0, strcmp(e.what(), "Empty dangers ratios set given to DangersCounter"));
    }catch(...){
        FAIL() << "Expected an out_of_range exception";
    }
}


TEST(DangersCounterTest, InvalidIDThrows) {
    const std::string expectedDangerID = "danger";
    try {
        jdb::DangersCounter dangersFactoryA(15, {{expectedDangerID, 1.0f}});
        dangersFactoryA.nDangers("another-danger");
        FAIL() << "Expected std::out_of_range from invalid danger ID";
    }catch(std::out_of_range const& e){
        EXPECT_NE(nullptr, strstr(e.what(), "another-danger")) << "Expected \"" + expectedDangerID + "\" in exception message";
    }catch(...){
        FAIL() << "Expected std::out_of_range";
    }
}


TEST(DangersCounterTest, DecreasingDangerWithInvalidIDThrows) {
    const std::string expectedDangerID = "danger";
    try {
        jdb::DangersCounter dangersFactory(15, {{expectedDangerID, 1.0f}});
        dangersFactory.decreaseDangerCounter("another-danger");
        FAIL() << "Expected std::out_of_range from invalid danger ID";
    }catch(std::out_of_range const& e){
        SUCCEED();
    }catch(...){
        FAIL() << "Expected std::out_of_range";
    }
}


TEST(DangersCounterTest, TryingToDecreaseANullDangerCounterThrows) {
    jdb::DangersCounter dangersFactory(1, {{"danger", 1.0f}});
    dangersFactory.decreaseDangerCounter("danger");
    EXPECT_EQ(0, dangersFactory.nDangers("danger"));
    try{
        dangersFactory.decreaseDangerCounter("danger");
        FAIL() << "Expected a runtime_error because of trying to decrease a zero counter";
    }catch(std::runtime_error& e){
        EXPECT_EQ(0, strcmp(e.what(), "Triying to decrease a zero counter"));
    }catch(...){
        FAIL() << "Expected a runtime_error";
    }
}


TEST(DangersCounterTest, ResetingCountesWorks){
    jdb::DangersCounter dangersCounter(3, {{"danger", 1.0f}});
    dangersCounter.decreaseDangerCounter("danger");
    EXPECT_EQ(2, dangersCounter.nDangers("danger"));
    dangersCounter.decreaseDangerCounter("danger");
    EXPECT_EQ(1, dangersCounter.nDangers("danger"));
    dangersCounter.reset();
    EXPECT_EQ(3, dangersCounter.nDangers("danger"));
}


TEST(DangersCounterTest, InitialNDangersGetterWorks){
    jdb::DangersCounter dangersCounter(10, {{"danger", 1.0f}});

    EXPECT_EQ(10, dangersCounter.initialNDangers());
    for(unsigned int i=0; i<3; i++ ){
        dangersCounter.decreaseDangerCounter("danger");
        EXPECT_EQ(10, dangersCounter.initialNDangers());
    }
}
