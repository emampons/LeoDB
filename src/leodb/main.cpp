#include <iostream>
#include "key.h"

int main() {
    Key<int> one (5);
    Key<int> two (6);
    std::cout << (one < two);
    return 0;
}