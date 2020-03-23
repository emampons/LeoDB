#include <iostream>
#include <db.cpp>
#include <data/entry.cpp>
#include <data/key.cpp>
#include <data/value.cpp>
#include <loguru.cpp>

int main() {
    DB<int, int> leodb;
    for (int x = 0; x < 500; x++){
        leodb.put(x, x);
    }
    leodb.close();
    return 0;
}