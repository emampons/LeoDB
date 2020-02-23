#include <iostream>
#include "key.h"
#include "db.h"
#include "value.h"
#include "value.cpp"
#include "db.cpp"
#include "entry.h"
#include "entry.cpp"

int main() {
    Key<int> one (5);
    Key<int> two (6);
    //std::cout << (one < two);
    DB<int, std::string> test;
    test.put(Key<int>(5), Value<std::string>("hi"));
    std::cout<<test.del(Key<int>(3));
    return 0;
}