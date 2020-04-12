#include <iostream>
#include <db.cpp>


int main() {
    DB<int, int> leodb;
    for (int x = 0; x < 499; x++){
        leodb.put(x, x);
    }
    leodb.del(0);
//    std::cout << leodb.get(3).getString();
    leodb.close();

    return 0;
}

/*
 * TODO:
 * - Tomb search for delete
 * - Fence pointer level update
 * - Optimizer function
 * - Final Testing
 */