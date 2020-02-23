#ifndef LEODB_DB_H
#define LEODB_DB_H

#include <unordered_map>
#include "key.h"
#include "value.h"
#include "entry.h"

template <class T, class U>
class DB {
public:
    DB() = default;;
    bool put(Key<T> key, Value<U> value);
    bool del(Key<T> key);
    Value<T> get(Key<T> key);
 //   template<class T>
//    std::vector<Value<T>> scan(Key<T> low, Key<T> high);
//    int min();
//    int max();
//    float avg();
//    float stddev();
private:
    std::unordered_map<int, Entry<T, U>> table;
    int totalKeys;
    // maybe some more stuff...
};

#endif //LEODB_DB_H
