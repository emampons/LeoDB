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
        leodb.put(rand_array[x], rand_array[x]);
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
//    std::cout << "Max: " + std::to_string(leodb.max());

    // Min
    std::cout << "Min: " + std::to_string(leodb.min()) + "\n";

    // Scan
    std::vector < Value<int> > ret = leodb.scan(Key<int>(0), Key<int>(500));
    for (auto item: ret){
        std::cout << item.getString() + "-";
    }

    std::cout << "\n";

    // Avg
    std::cout << "Avg: " + std::to_string(leodb.avg()) + "\n";

    // StdDev
    std::cout << "StdDev: " + std::to_string(leodb.stddev()) + "\n";

    leodb.close();
    return 0;
}
