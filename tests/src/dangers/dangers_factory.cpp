#include <dangers/dangers_factory.hpp>
#include <stdexcept>
#include <gtest/gtest.h>

TEST(DangersFactoryTest, nDangersIsInitializedInConstructor) {
    std::vector<int> nDangersVector = {5, 1, 15};
    for( int nDangers : nDangersVector ){
        jdb::DangersFactory dangersFactoryA(nDangers);
        EXPECT_EQ(nDangers, dangersFactoryA.nDangers());
    }
}



TEST(DangersFactoryTest, CantCreateEmptyDangersFactory){
    try{
        jdb::DangersFactory emptyDangersFactory(0);
        FAIL() << "Expected std::out_of_range";
    }catch(std::out_of_range const&){
        SUCCEED();
    }catch(...){
        FAIL() << "Expected std::out_of_range";
    }
}
