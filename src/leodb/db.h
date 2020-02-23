#ifndef LEODB_DB_H
#define LEODB_DB_H

#include <unordered_map>
#include "key.h"
#include "value.h"

template <class T>
struct hashFunction {
    // Use sum of lengths of first and last names
    // as hash function.
    size_t operator()(const Key<T>& key) const
    {
        return key.hashItem();
    }
};

class DB {
public:
    DB() = default;;
    template<class T>
    bool put(Key<T> key, Value<T> value);
    template<class T>
    bool del(Key<T> key);
    template<class T>
    Value<T> get(Key<T> key);
 //   template<class T>
//    std::vector<Value<T>> scan(Key<T> low, Key<T> high);
//    int min();
//    int max();
//    float avg();
//    float stddev();
private:

    std::unordered_map<Key, Value, hashFunction> table;
    int totalKeys;
    // maybe some more stuff...
};

//template <class T>
//struct hashFunction {
//    // Use sum of lengths of first and last names
//    // as hash function.
//    size_t operator()(const Key<T>& key) const
//    {
//        return key.hashItem();
//    }
//};

#endif //LEODB_DB_H
