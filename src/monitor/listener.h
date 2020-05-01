#ifndef LEODB_LISTENER_H
#define LEODB_LISTENER_H

#include <string>
#include <unordered_map>
#include <loguru.cpp>

// Global Variables
std::string STARTING_LEVEL = "tier"; // Start by assuming low reads

class Listener {
public:
    Listener();
    ~Listener() = default;

    std::unordered_map<std::string, std::string>  optimize(std::unordered_map<std::string, std::string> level_info);
    void set_tune(bool _tune);
    void set_starting_level(std::string _starting_level);
    std::string get_starting_level();
    void monitor(std::string command);
private:
    std::string starting_level;
    double T = 2;
    int counter;
    bool tune;
    std::unordered_map<std::string, int> tracker;
    std::unordered_map<std::string, std::string> level_map;
    std::unordered_map<std::string, std::string>  optimize_level(std::unordered_map<std::string, std::string> level_info);
};


#endif //LEODB_LISTENER_H
