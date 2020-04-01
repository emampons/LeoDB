#include <iostream>
//#include <data/value.cpp>
#include <gtest/gtest.h>
#include <db.cpp>


class ValueTest : public ::testing::Test {
protected:
    std::vector<Value<int> > values;

    void SetUp() override {
        for (int x = 0; x < 10; x++) {
            values.emplace_back(Value<int>(x));
        }
    }
};


TEST_F(ValueTest, ConstructorFunctionality) {
    for(int i =0; i< 10; i++){
        Value<int> temp = values[i];
        EXPECT_EQ(temp.getItem(), i);
    }

}


TEST_F(ValueTest, LessThanFunctionality) {
    Value<int> previous = values[0];
    for(int i =1; i< 10; i++){
        Value<int> curr = values[i];
        EXPECT_EQ(previous < curr, true);
        previous = values[i];
    }

}

TEST_F(ValueTest, LessThanEqFunctionality) {
    Value<int> previous = values[0];
    for(int i =1; i< 10; i++){
        Value<int> curr = values[i];
        EXPECT_EQ(previous <= curr, true);
        previous = values[i];
    }

}

TEST_F(ValueTest, GreaterThanFuncitonality) {
    Value<int> previous = values[9];
    for(int i =8; i>0; i--){
        Value<int> curr = values[i];
        EXPECT_EQ(previous > curr, true);
        previous = values[i];
    }

}

TEST_F(ValueTest, GreaterThanEqFuncitonality) {
    Value<int> previous = values[9];
    for(int i =8; i>0; i--){
        Value<int> curr = values[i];
        EXPECT_EQ(previous >= curr, true);
        previous = values[i];
    }

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}