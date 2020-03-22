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
    DB<int, int> leodb;
    leodb.DUMP_MANIFEST();
    //test.CLOSE();
    //    test.put(Key<int>(10), Value<int>(32));
//    leodb.put(1, 1);
    std::cout << (int) leodb.getHeight();


    return 0;
}