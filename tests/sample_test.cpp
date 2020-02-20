#include <iostream>
#include <vector>

#include "gtest/gtest.h"



class DBTest : public ::testing::Test{
protected:
    std::vector<int> db1;




    void SetUp() override
    {
        db1.push_back(2);
        db1.push_back(5);
    }
};


TEST_F(DBTest, IsEmptyInitially) {
EXPECT_EQ(db1.size(), 0);
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}