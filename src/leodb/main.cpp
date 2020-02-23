#include <iostream>
#include "key.h"
#include "db.h"
int main() {
    Key<int> one (5);
    Key<int> two (6);
    std::cout << (one < two);
    DB<int> test;
    return 0;
}