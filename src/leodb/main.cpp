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
    test.put(Key<int>(1), Value<int>(3));
    test.put(Key<int>(10), Value<int>(32));

    std::cout<<test.stddev(false);
    return 0;
}