#include "listener.h"

void Listener::monitor(std::string command) {
    /*
     * Function monitor: Helper function to track reads and writes
     * Param string command: Type of command (i.e. READ, WRITE)
     */
    tracker[command] = tracker[command] + 1;
}

std::unordered_map<std::string, std::string>  Listener::optimize(std::unordered_map<std::string, std::string> level_info) {
    /*
     * Function optimize: Function to consider a level_info, consult recorded data and tune the level accordingly
     * Param unordered_map<string, string> level_info: Information on the current level we are on
     * Returns: Updated level_info
     */
    // Extract information
    int level_reads = std::stoi(level_info["LevelReads"]);
    int writes = tracker["WRITES"];
    int reads = tracker["READS"];

    // TODO: The hard part :(

    return level_info;
}

void Listener::set_tune(bool _tune) {
    tune = _tune;
}
