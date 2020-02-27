#include <iostream>
#include "../src/leodb/db.h"
#include "../src/leodb/db.cpp"
#include "../src/leodb/key.h"
#include "../src/leodb/key.cpp"
#include "../src/leodb/value.h"
#include "../src/leodb/value.h"
#include "../src/leodb/entry.h"
#include "../src/leodb/entry.cpp"


bool driver(){
    // Create a DB object
    DB<int, int> db = DB<int, int>();
    bool ret = true;

    std::cout << "[INFO] Testing .put() function...\n";
    try {
        for (int x = 0; x < 100; x++){
            db.put(Key<int>(x), Value<int>(x+100));
        };
        int size = db.size();
        bool passed = (size == 100);
        if (!passed) {
            std::cout << "[FAILED] Failed on put function\n";
            ret = false;
        };
    } catch(int e) {
        std::cout << "[FAILED] Failed on put function\n";
        ret = false;
    };

    std::cout << "[INFO] Testing .get() function...\n";
    try {
        int value = db.get(Key<int>(99)).getItem();
        bool passed = (value == 199);
        if (!passed) {
            std::cout << "[FAILED] Failed on .get() function\n";
            ret = false;
        };
    } catch(int e) {
        std::cout << "[FAILED] Failed on .get() function\n";
        ret = false;
    }

    std::cout << "[INFO] Testing .min() and .max() function...\n";
    try {
        int min_keys = db.min();
        int max_keys = db.max();
        int min_value = db.min(false);
        int max_value = db.max(false);
        bool passed_keys = (min_keys == 0 && max_keys == 99);
        bool passed_value = (min_value == 100 && max_value == 199);
        bool passed = (passed_keys && passed_value);

        if (!passed) {
            std::cout << "[FAILED] Failed on .min() or .max() function\n";
            ret = false;
        };
    } catch(int e) {
        std::cout << "[FAILED] Failed on .min() or .max() function\n";
        ret = false;
    }

    std::cout << "[INFO] Testing .avg() function...\n";
    try {
        float avg_keys = db.avg();
        float avg_values = db.avg(false);
        bool passed = (avg_keys == 49.5 && avg_values == 149.5);

        if (!passed) {
            std::cout << "[FAILED] Failed on .avg() function\n";
            ret = false;
        };
    } catch(int e) {
        std::cout << "[FAILED] Failed on .avg() function\n";
        ret = false;
    }

    std::cout << "[INFO] Testing .stddev() function...\n";
    try {
        std::string stddev_keys = std::to_string(db.stddev());
        std::string stddev_values = std::to_string(db.stddev(false));
        bool passed = (stddev_keys.compare("29.011492") == 0 && stddev_values.compare("29.011492") == 0);

        if (!passed) {
            std::cout << "[FAILED] Failed on .stddev() function\n";
            ret = false;
        };
    } catch(int e) {
        std::cout << "[FAILED] Failed on .stddev() function\n";
        ret = false;
    }

    std::cout << "[INFO] Testing .scan() function...\n";
    try {
        std::vector<Value<int> > response = db.scan(Key<int>(10), Key<int>(20));
        int start = 120;
        bool passed = true;
        for (auto value: response) {
            if (value.getItem() != start) {
                passed = false;
            }
            start--;
        }
        if (!passed) {
            std::cout << "[FAILED] Failed on .scan() function\n";
            ret = false;
        };
    } catch(int e) {
        std::cout << "[FAILED] Failed on .scan() function\n";
        ret = false;
    }

    std::cout << "[INFO] Testing .delete() function...\n";
    try {
        for (int x = 20; x <= 40; x++) {
            db.del(Key<int>(x));
        }
        int size = db.size();
        bool passed = (size == 79);

        if (!passed) {
            std::cout << "[FAILED] Failed on .delete() function\n";
            ret = false;
        };
    } catch(int e) {
        std::cout << "[FAILED] Failed on .delete() function\n";
        ret = false;
    }

    return ret;
}



int main() {
    bool ret = driver();
    if (ret) {
        std::cout << "[INFO] Tests have passed! :)\n";
        return 0;
    } else {
        std::cout << "[INFO] Tests have Failed! :(\n";
        return 1;
    }
};