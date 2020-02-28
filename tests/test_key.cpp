#include <iostream>
#include "../src/leodb/key.h"
#include "../src/leodb/key.cpp"
#include "gtest/gtest.h"


class KeyTest : public ::testing::Test {
protected:
    std::vector<Key<int> > keys;

    void SetUp() override {
        for (int x = 0; x < 10; x++) {
            keys.emplace_back(Key<int>(x));
        }
    }
};


TEST_F(KeyTest, ConstructorFunctionality) {
    for(int i =0; i< 10; i++){
        Key<int> temp = keys[i];
        EXPECT_EQ(temp.getItem(), i);
    }

}

TEST_F(KeyTest, EqualTest){
    for(int i =0; i< 10; i++){
        Key<int> temp = keys[i];
        Key<int> comp(i);
        EXPECT_EQ(comp == temp, true);
    }
}

TEST_F(KeyTest, LessThanFunctionality) {
    Key<int> previous = keys[0];
    for(int i =1; i< 10; i++){
        Key<int> curr = keys[i];
        EXPECT_EQ(previous < curr, true);
        previous = keys[i];
    }

}

TEST_F(KeyTest, LessThanEqFunctionality) {
    Key<int> previous = keys[0];
    for(int i =1; i< 10; i++){
        Key<int> curr = keys[i];
        EXPECT_EQ(previous <= curr, true);
        previous = keys[i];
    }

}

TEST_F(KeyTest, GreaterThanFuncitonality) {
    Key<int> previous = keys[9];
    for(int i =8; i>0; i--){
        Key<int> curr = keys[i];
        EXPECT_EQ(previous > curr, true);
        previous = keys[i];
    }

}

TEST_F(KeyTest, GreaterThanEqFuncitonality) {
    Key<int> previous = keys[9];
    for(int i =8; i>0; i--){
        Key<int> curr = keys[i];
        EXPECT_EQ(previous >= curr, true);
        previous = keys[i];
    }

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}