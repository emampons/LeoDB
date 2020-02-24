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
    Value<int> val (4);
    Key<int> two (6);
    Entry<int, int> hello(one,val);

    DB<int, int> test;
    test.put(Key<int>(5), Value<int>(3));
    test.put(Key<int>(4), Value<int>(32));
    test.put(Key<int>(6), Value<int>(3132));
    test.put(Key<int>(7), Value<int>(33));
    test.put(Key<int>(8), Value<int>(36));

    std::cout<<(test.scan(Key<int>(4), Key<int>(5)).size());
    return 0;
}