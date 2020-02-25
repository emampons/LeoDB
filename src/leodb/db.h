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
    std::vector<Value<T> > scan(Key<T> low, Key<T> high);
    int min(bool keys=true);
    int max(bool keys=true);
    float avg(bool keys=true);
    float stddev(bool keys=true);
private:
    std::unordered_map<int, Entry<T, U> > table;
    int totalKeys;
    // maybe some more stuff...
};

#endif //LEODB_DB_H
