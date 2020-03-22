#include <iostream>
#include <db.cpp>
#include <data/entry.cpp>
#include <data/key.cpp>
#include <data/value.cpp>
#include <loguru.cpp>

int main() {
//    Key<int> one (5);
//    Value<int> val (4);
//    Key<int> two (6);
    DB<int, int> test;
    DB<int, int> test2;
    bool ret = test.put(Key<int>(1), Value<int>(3));
    test2.LOAD_FROM_FILE("cmake-build-debug/data.txt");
    //test.CLOSE();
//    test.put(Key<int>(10), Value<int>(32));

    std::cout<<test2.get(Key<int>(1)).getItem();
    loguru::add_file("data/leodb-lob.log", loguru::Append, loguru::Verbosity_MAX);
    LOG_F(INFO, "Hello log file!");
    return 0;
}