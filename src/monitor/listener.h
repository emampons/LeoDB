#ifndef LEODB_LISTENER_H
#define LEODB_LISTENER_H

#include <string>
#include <unordered_map>
#include <loguru.cpp>

class Listener {
public:
    Listener() = default;
    ~Listener() = default;

    std::unordered_map<std::string, std::string>  optimize(std::unordered_map<std::string, std::string> level_info);
    void set_tune(bool _tune);
    void monitor(std::string command);
private:
    double T = 2;
    bool tune;
    std::unordered_map<std::string, int> tracker;
};


#endif //LEODB_LISTENER_H
