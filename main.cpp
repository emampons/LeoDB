#include <iostream>
#include <db.cpp>
#include <data/entry.cpp>
#include <data/key.cpp>
#include <data/value.cpp>


int main() {
//    Key<int> one (5);
//    Value<int> val (4);
//    Key<int> two (6);
    DB<int, int> test;
    bool ret = test.put(Key<int>(1), Value<int>(3));
    std::cout << ret;
//    test.put(Key<int>(10), Value<int>(32));
//
//    std::cout<<test.stddev(false);
    return 0;
}