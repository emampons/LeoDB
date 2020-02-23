#ifndef LEODB_DB_H
#define LEODB_DB_H

#include <unordered_map>
#include "key.h"
#include "value.h"

template<class T>
class DB {
public:
    DB() {};
    bool put(Key<T> key, Value value);
    bool del(Key<T> key);
    Value get(Key<T> key);
    std::vector<Value> scan(Key<T> low, Key<T> high);
    int min();
    int max();
    float avg();
    float stddev();
private:
    std::unordered_map<Key<T>, Value, hashFunction<T>> table;
    int totalKeys;
    // maybe some more stuff...
};

class hashFunction {
public:

    // Use sum of lengths of first and last names
    // as hash function.
    size_t operator()(const Key<T>& key) const
    {
        return key.hashItem();
    }
};

#endif //LEODB_DB_H
