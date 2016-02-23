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


TEST(DangersCounterTest, DangersTotalIsDistributedBetweenSpecificDangers) {
    unsigned int nDangers = 15;
    jdb::DangersCounter dangersFactory(nDangers, {{"dangerA", 1}, {"dangerB", 1}, {"dangerC", 1}});
    EXPECT_EQ(nDangers / 3, dangersFactory.nDangers("dangerA"));
    EXPECT_EQ(nDangers / 3, dangersFactory.nDangers("dangerB"));
    EXPECT_EQ(nDangers / 3, dangersFactory.nDangers("dangerC"));
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
