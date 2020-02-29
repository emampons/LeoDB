#include <iostream>
#include <db.cpp>
#include <data/entry.cpp>
#include <data/key.cpp>
#include <data/value.cpp>
#include <gtest/gtest.h>


class OperatorsTest : public ::testing::Test {
protected:
    DB<int, int> db = DB<int, int>();

    void SetUp() override {
        for (int x = 0; x < 100; x++) {
            db.put(Key<int>(x), Value<int>(x + 100));
        }
    }
};


TEST_F(OperatorsTest, IsPutWorking) {
    EXPECT_EQ(db.size(), 100);
}


TEST_F(OperatorsTest, GetFunctionality) {
    int value = db.get(Key<int>(99)).getItem();

    EXPECT_EQ(value, 199);
}


TEST_F(OperatorsTest, MinMaxFunctionality) {
    int min_keys = db.min();
    int max_keys = db.max();
    int min_value = db.min(false);
    int max_value = db.max(false);


    EXPECT_EQ(min_keys, 0);
    EXPECT_EQ(max_keys, 99);
    EXPECT_EQ(min_value, 100);
    EXPECT_EQ(max_value, 199);
}


TEST_F(OperatorsTest, AverageFunctionality) {
    float avg_keys = db.avg();
    float avg_values = db.avg(false);
    EXPECT_EQ(avg_keys, 49.5);
    EXPECT_EQ(avg_values, 149.5);
}

TEST_F(OperatorsTest, StdDevFunctionality) {
    std::string stddev_keys = std::to_string(db.stddev());
    std::string stddev_values = std::to_string(db.stddev(false));

    EXPECT_EQ(stddev_keys, "29.011492");
    EXPECT_EQ(stddev_values, "29.011492");
}

TEST_F(OperatorsTest, ScanFunctionality) {
    std::vector<Value<int> > response = db.scan(Key<int>(10), Key<int>(20));
    std::vector<Value<int> > correct_range;
    for(int x = 120; x >=110; x--){
        correct_range.emplace_back(x);
    }
    EXPECT_EQ(response, correct_range);
}

TEST_F(OperatorsTest, DeleteFuncitonality) {
    for (int x = 20; x <= 40; x++) {
        db.del(Key<int>(x));
    }
    int size = db.size();
    EXPECT_EQ(size, 79);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}