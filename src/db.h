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
#include <filesystem>

typedef enum status{
    OPEN = 0,
    CLOSED = 1,
    BUSY = 100,
    ERROR = 400,
} DB_STATUS;

// Global Constants
std::string DATA_FOLDER_PATH = "data";
std::string LOGGER_PATH = DATA_FOLDER_PATH + "/leodb-log.log";

// TODO: Make this dynamic
std::string MAX_RUNS = "3";
int IN_MEMORY_THRESHOLD = 10;

template <class T, class U>
class DB {
public:
    // Constructor
    DB(std::string file_path=DATA_FOLDER_PATH, std::string logger_path=LOGGER_PATH);

    // Basic Operations
    bool put(T _key, U _value);
    bool del(T _key);
    Value<T> get(T _key);
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

    // DB Information
    int size();
    int getHeight();
    void close();

private:
    // Private Variables
    int MEMORY_THRESHOLD;
    int totalKeys;
    std::unordered_map<int, Entry<T, U> > table;
    std::unordered_map<std::string, std::string> manifest;
    std::ofstream file;

    // Private Functions
    // Basic Operations
    Value<U> SEARCH_MEMORY(Key<T> key);

    // Logistics
    bool DUMP_MANIFEST(std::string file_path=DATA_FOLDER_PATH);
    bool LOAD_MANIFEST(std::string file_path=DATA_FOLDER_PATH);
    std::unordered_map<std::string, std::string> LOAD_LEVEL(std::string level, std::string type);
    bool DUMP_LEVEL(std::unordered_map<std::string, std::string> level_info);
    bool WRITE_TO_FILE();
    std::string initialize_manifest();
    std::string initialize_level(std::string level, std::string type, std::string max_pairs);
    void DUMP_IN_MEMORY();
    void LOAD_IN_MEMORY();

    // Tier vs Leveling
    std::vector<std::pair<int, Entry<T, U> > > load_level_data(std::unordered_map<std::string, std::string> level_info);
    std::vector<std::pair<int, Entry<T, U> > > load_tier_data(std::unordered_map<std::string, std::string> level_info);


    // Flushing
    void flush_new_level(std::unordered_map<std::string, std::string> level_info, std::vector<std::pair<int, Entry<T, U> > > sorted);
    void add_data_to_level(std::unordered_map<std::string, std::string> level_info, std::string output);
    void delete_level_content(std::unordered_map<std::string, std::string> level_info);
    int get_lines_in_file(std::string file_path);

    struct {
        // Helper function to compare pairs
        bool operator()(const std::pair<int, Entry<T, U> > &a, const std::pair<int, Entry<T, U> > &b) const {
            return a.first < b.first;
        }
    } customLess;
};

#endif //LEODB_DB_H
