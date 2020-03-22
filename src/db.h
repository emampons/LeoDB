#ifndef LEODB_DB_H
#define LEODB_DB_H

#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <data/key.h>
#include <data/value.h>
#include <data/entry.h>
#include <loguru.cpp>

typedef enum status{
    OPEN = 0,
    CLOSED = 1,
    BUSY = 100,
    ERROR = 400,
} DB_STATUS;

// Global Constants
std::string DATA_FOLDER_PATH = "data";
std::string LOGGER_PATH = DATA_FOLDER_PATH + "/leodb-log.log";

template <class T, class U>
class DB {
public:
    // Constructor
    DB(std::string file_path=DATA_FOLDER_PATH, std::string logger_path=LOGGER_PATH);

    // Basic Operations
    bool put(int key, int value);
    bool put(Key<T> key, Value<U> value);
    bool del(Key<T> key);
    Value<T> get(Key<T> key);
    std::vector<Value<T> > scan(Key<T> low, Key<T> high);
    int min(bool keys=true);
    int max(bool keys=true);
    float avg(bool keys=true);
    float stddev(bool keys=true);

    // Logistics
    DB_STATUS db_status;
    DB_STATUS OPEN_FILE();
    bool CLOSE();
    bool LOAD_FROM_FILE(std::string file_name);
    bool DUMP_MANIFEST(std::string file_path=DATA_FOLDER_PATH);
    bool LOAD_MANIFEST(std::string file_path=DATA_FOLDER_PATH);

    // DB Information
    int size();
    int getHeight();

private:
    // Private Variables
    int MEMORY_THRESHOLD;
    int totalKeys;
    std::unordered_map<int, Entry<T, U> > table;
    std::unordered_map<std::string, std::string> manifest;
    std::ofstream file;

    // Private Function
    bool WRITE_TO_FILE();
    std::string initialize_manifest();
};

#endif //LEODB_DB_H
