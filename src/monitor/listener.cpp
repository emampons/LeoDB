#include <monitor/listener.h>


Listener::Listener() {
    /*
    * Function constructor: Constructor for our listener
    */
    counter = 0;
    tracker["MaxLevelReads"] = 0;

}

void Listener::set_starting_level(std::string _starting_level) {
    LOG_F(WARNING, ("Using starting_level: " + _starting_level).c_str());
    starting_level = _starting_level;
}
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
    if (!tune){
        return level_info;
    }
    // Extract information
    int level_reads = std::stoi(level_info["LevelReads"]);
    int writes = tracker["WRITES"];
    int reads = tracker["READS"];
    if (level_info["CurrentRun"].compare("0") == 0 && level_info["level"].compare("0") != 0) {
        // Only Optimize levels when they are new and not level 0,
        // not when they have already been declared (level 0 controlled by db.cpp, see set_starting_level)

        // Save metadata
        tracker["MaxLevelReads"] = std::max(level_reads, tracker["MaxLevelReads"]);

        if (level_map.find(level_info["level"]) != level_map.end()){
            // We have already optimized this level, return that config
            level_info["Type"] = level_map[level_info["level"]];
        } else {
            // First time seeing this level, optimize it accordingly
            LOG_F(INFO, ("Optimizing level " + level_info["level"]).c_str());

            // Modify the level accordingly
            level_info = optimize_level(level_info);

            // Update our map so we can keep this configuration when we see this level again
            level_map[level_info["level"]] = level_info["Type"];
        }
    }
    return level_info;
}

std::string Listener::get_starting_level() {
    /*
     * Function get_starting_level: Helper function to return the root/starting level type
     * Returns: String of the starting level type (i.e. tier/level)
     */
    if (starting_level.compare("any_level") == 0 ){
        return "tier";
    }
    return starting_level;
}

std::unordered_map<std::string, std::string>  Listener::optimize_level(std::unordered_map<std::string, std::string> level_info) {
    /*
     * Function optimize_level: Helper function to determine how to optimize a level
     * Param unordered_map<string, string> level_info: Information on the current level we are on
     * Returns: Updated level_info
     */
    if (starting_level.compare("any_level") == 0 ){
        // If the level has some reads convert it to level level, else keep as tier
        int level_reads = std::stoi(level_info["LevelReads"]);
        if (level_reads > 0 ) {
            level_info["Type"] = "level";
        } else {
            level_info["Type"] = "tier";
        }
        return level_info;
    } else {
        level_info["Type"] = get_starting_level();
        return level_info;
    }
}

//std::unordered_map<std::string, std::string>  Listener::optimize_level(std::unordered_map<std::string, std::string> level_info) {
//    /*
//     * Function optimize_level: Helper function to determine how to optimize a level
//     * Param unordered_map<string, string> level_info: Information on the current level we are on
//     * Returns: Updated level_info
//     */
//    // If the level has some reads convert it to level level, else keep as tier
//    int level_reads = std::stoi(level_info["LevelReads"]);
//    if (level_reads > 0 ) {
//        level_info["Type"] = "level";
//    } else {
//        level_info["Type"] = "tier";
//    }
//    return level_info;
//}

void Listener::set_tune(bool _tune) {
    /*
     * Function set_tune: Enable or disable turning
     * Param bool _tune: True/False if we should/shouldn't enable turning
     */
    if (!_tune)
        LOG_F(WARNING, "Turning disabled");
    tune = _tune;
}
