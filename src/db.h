#ifndef LEODB_DB_H
#define LEODB_DB_H

#include <unordered_map>
#include <fstream>
#include <sstream>
#include <data/key.h>
#include <data/value.h>
#include <data/entry.h>

typedef enum status{
    OPEN = 0,
    CLOSED = 1,
    BUSY = 100,
    ERROR = 400,
} DB_STATUS;

template <class T, class U>
class DB {
public:
    DB() = default;
    bool put(Key<T> key, Value<U> value);
    bool del(Key<T> key);
    Value<T> get(Key<T> key);
    std::vector<Value<T> > scan(Key<T> low, Key<T> high);
    int min(bool keys=true);
    int max(bool keys=true);
    float avg(bool keys=true);
    float stddev(bool keys=true);
    int size();
    DB_STATUS db_status;
    DB_STATUS OPEN_FILE();
    bool CLOSE();
    bool LOAD_FROM_FILE(const std::string& file_name);
private:
    std::unordered_map<int, Entry<T, U> > table;
    int totalKeys;
    std::ofstream file;
    int MEMORY_THRESHOLD = 10000;
    bool WRITE_TO_FILE();
    // maybe some more stuff...
};

#endif //LEODB_DB_H
