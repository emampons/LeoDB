#include <iostream>
#include <db.cpp>


int main() {
    DB<int, int> leodb;
    for (int x = 0; x < 509; x++){
        leodb.put(x, x);
    }
    leodb.del(0);
    leodb.del(1);
    leodb.del(2);
//    leodb.get(0);
//    std::cout << leodb.get(75).getTomb();
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