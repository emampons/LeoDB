#include <iostream>
#include <db.cpp>
#include <loguru.cpp>

int main() {
    int rand_array[1000];
    for(int i=1;i<=1000;i++){
        rand_array[i]=(std::rand() % 1000) + 1;
    }

    DB<int, int> leodb("level", "data", "data/leodb-log.log");
    for (int x = 0; x < 1000; x++){
        leodb.put(x, x);
    }

    // Delete
//    leodb.del(0);
//    Value<int> ret = leodb.get(0);
//    std::cout << std::to_string(ret.getTomb());
//    if (ret.getTomb()){
//        std::cout << "Value has been deleted";
//    } else {
//        std::cout << "Value exists!";
//    }

    // Max
//    std::cout << leodb.max();

    // Min
//    std::cout << leodb.min();

    // Scan
//    std::vector < Value<int> > ret = leodb.scan(Key<int>(0), Key<int>(500));
//    for (auto item: ret){
//        std::cout << item.getString() + "-";
//    }

    // Avg
//    std::cout << leodb.avg();

    // StdDev
//    std::cout << leodb.stddev();

    leodb.close();
    return 0;
}

/*
 * Working:
 * - Put
 * - Get
 * - Delete
 *
 */