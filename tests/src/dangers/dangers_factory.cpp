#include <dangers/dangers_factory.hpp>
#include <stdexcept>
#include <gtest/gtest.h>

TEST(DangersFactoryTest, nDangersIsInitializedInConstructor) {
    std::vector<int> nDangersVector = {5, 1, 15};
    std::unique_ptr<std::vector<jdb::DangerData>> dangersData;
    for( int nDangers : nDangersVector ){
        jdb::DangersFactory dangersFactoryA(nDangers, std::move(dangersData));
        EXPECT_EQ(nDangers, dangersFactoryA.nDangers());
    }
}


TEST(DangersFactoryTest, CantCreateEmptyDangersFactory){
    try{
        std::unique_ptr<std::vector<jdb::DangerData>> dangersData;
        jdb::DangersFactory emptyDangersFactory(0, std::move(dangersData));
        FAIL() << "Expected std::out_of_range";
    }catch(std::out_of_range const&){
        SUCCEED();
    }catch(...){
        FAIL() << "Expected std::out_of_range";
    }
}
