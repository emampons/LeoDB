#include <cmath>
#include <db.h>


template<class T, class U>
DB<T, U>::DB(std::string file_path, std::string logger_path) {
    /*
     * Function constructor: Start up our DB
     */
    // Load our logger
    loguru::add_file(logger_path.c_str(), loguru::Append, loguru::Verbosity_MAX);
    LOG_F(INFO, "Logger Loaded - Starting LeoDB!");

    // Initialize local variables
    totalKeys = 0;
    MEMORY_THRESHOLD = IN_MEMORY_THRESHOLD;

    // Load our Manifest file
    LOG_F(INFO, "Attempting to load manifest file...");
    LOAD_MANIFEST(file_path.c_str());

    // Load our in-memory data
    LOG_F(INFO, "Attempting to load previous in-memory data...");
    LOAD_IN_MEMORY();
}

template<class T, class U>
bool DB<T, U>::put(T _key, U _value) {
    /*
     * Function put: Takes in a Key and Value and inserts into our db
     * Param Key _key: Key to insert
     * Param Value _value: Value to insert
     * Return: True/False if it was successful
     */
    Key<T> key(_key);
    Value<U> value(_value);
    int inserted = key.hashItem();
    if (table.count(inserted) == 0) {
        totalKeys += 1;
    }
    table[inserted] = Entry<T, U>(key, value);
    DLOG_F(INFO, ("Added new Key/Value pair, Hash::Key:::Value->" + table[inserted].buildString()).c_str());

    if (totalKeys >= MEMORY_THRESHOLD) {
        INIT_WRITE_TO_FILE();
    }
    return true;
}


template<class T, class U>
bool DB<T, U>::del(T _key) {
    /*
     * Function del: Delete a key from our DBMS
     * Param Key _key: Key to use for lookup
     * Return: True/False if it was successful
     */
    Key<T> key (_key);
    if (table.count(key.hashItem())){
        // Delete it from the in-memory table
        table.erase(key.hashItem());
    } else {
        // Else insert the special deleted Entry
        put(key, NULL);
    }
    DLOG_F(INFO, ("Deleted Key: " + key.getString()).c_str());
    return true;
}

template<class T, class U>
Value<T> DB<T, U>::get(T _key) {
    /*
     * Function get: Get a Value from a Key in our DBMS
     * Param Key _key: Key to use for lookup
     * Return: Value that was found or null if not found
     */
    Key<T> key (_key);
    if (table.count(key.hashItem())){
        // Delete it from the in-memory table
        return table[key.hashItem()].getValue();
    } else {
        // Else the key is in memory
        if(bloom.query(key.hashItem())){
           Value<T> ret = fence.search(key);
            return ret;
        }
        else{
            return SEARCH_MEMORY(key);
        }

    }
}

template<class T, class U>
std::vector<Value<T> > DB<T, U>::scan(Key<T> low, Key<T> high) {
    /*
     * Function scan: Get a Value from a Key in our DBMS
     * Param Key low: Lower-bound to scan
     * Param Key high: Higher-bound to scan
     * Return: Vector of values related to Keys between low/high
     */
    std::vector<Value<T> > ret;

    for (auto pair: table) {
        Entry<T, U> entry = pair.second;
        if ((entry.getKey() >= low) && (entry.getKey() <= high))
            ret.push_back(entry.getValue());
    }
    return ret;
}

template<class T, class U>
int DB<T, U>::min(bool keys) {
    /*
     * Function min: Get the min value in our DBMS
     * Param bool key: Flag to indicate if we should search the keys or values
     * Return: Int representing the min value
     */
    int min = INT_MAX;
    if (keys) {
        for (auto pair: table) {
            Entry<T, U> entry = pair.second;
            if (entry.getKey().getItem() < min) {
                min = entry.getKey().getItem();
            }
        }

    } else {
        for (auto pair: table) {
            Entry<T, U> entry = pair.second;
            if (entry.getValue().getItem() < min) {
                min = entry.getValue().getItem();
            }
        }
    }

    return min;
}

template<class T, class U>
int DB<T, U>::max(bool keys) {
    /*
     * Function max: Get the max value in our DBMS
     * Param bool key: Flag to indicate if we should search the keys or values
     * Return: Int representing the max value
     */
    int max = INT_MIN;
    if (keys) {
        for (auto pair: table) {
            Entry<T, U> entry = pair.second;
            if (entry.getKey().getItem() > max) {
                max = entry.getKey().getItem();
            }
        }

    } else {
        for (auto pair: table) {
            Entry<T, U> entry = pair.second;
            if (entry.getValue().getItem() > max) {
                max = entry.getValue().getItem();
            }
        }
    }

    return max;
}

//
template<class T, class U>
float DB<T, U>::avg(bool keys) {
    /*
     * Function avg: Get the avg value in our DBMS
     * Param bool key: Flag to indicate if we should search the keys or values
     * Return: Float representing the average of all values
     */
    float running_sum = 0.0;
    for (auto pair: table) {
        Entry<T, U> entry = pair.second;
        if (keys) {
            running_sum += entry.getKey().getItem();
        } else {
            running_sum += entry.getValue().getItem();
        }
    }
    return (running_sum / ((float) totalKeys));
}

template<class T, class U>
float DB<T, U>::stddev(bool keys) {
    /*
     * Function stddev: Get the standard deviation for the data
     * Param bool key: Flag to indicate if we should search the keys or values
     * Return: Float representing the standard deviation of all values
     */
    // First get the average
    float average = avg(keys);

    // Loop over and sum the difference squared
    float running_sum = 0.0;
    for (auto pair: table) {
        Entry<T, U> entry = pair.second;
        if (keys) {
            running_sum += pow((entry.getKey().getItem() - average), 2);
        } else {
            running_sum += pow((entry.getValue().getItem() - average), 2);
        }
    }
    return sqrt(running_sum / ((float) totalKeys - 1));
}

template<class T, class U>
int DB<T, U>::size() {
    /*
     * Function size: Get the size of the DB
     * Return: Int representing the size
     */
    return totalKeys;
}

template<class T, class U>
DB_STATUS DB<T, U>::OPEN_FILE() {
    /*
     * Function OPEN_FILE: Open data.txt file
     */
    this->file.open("data.txt", std::ios::out);

    if (file.is_open()) {
        db_status = OPEN;
        return db_status;
    } else if (!file) {
        this->file.open("data.txt", std::ios_base::app);
        db_status = OPEN;
    } else {
        file.close();
        db_status = ERROR;
    }
    return db_status;
}

template<class T, class U>
Value<U> DB<T, U>::SEARCH_MEMORY(Key<T> key){
    /*
     * Function SEARCH_MEMORY: Searchers memory for a key
     * Param Key key: Key we are looking for
     */
    // Hash our key
    int key_hash = key.hashItem();

    // Loop over all levels until we find the hash
    for (int x = 0; x < std::stoi(manifest["Height"]); x++){
        std::unordered_map<std::string, std::string> current_level = LOAD_LEVEL(std::to_string(x), "Level");
        std::vector<std::pair<int, Entry<T, U>>> values;
        if (current_level["Type"] == "Tier"){
            values = load_tier_data(current_level);
        } else if (current_level["Type"] == "Level") {
            values = load_level_data(current_level);
        }
        for (auto pair: values){
            if(pair.first == key_hash){
                // We found our value
                return pair.second.getValue();
            }
        }
    }
    LOG_F(WARNING, ("No value found for key " + key.getString()).c_str());
    return Value<U>(NULL);
}

template<class T, class U>
bool DB<T, U>::CLOSE() {
    OPEN_FILE();
    if (file.is_open()) {
        INIT_WRITE_TO_FILE();
        file.close();
    }

    db_status = CLOSED;
}

template<class T, class U>
bool DB<T, U>::LOAD_FROM_FILE(std::string file_name) {
    std::ifstream fid(file_name.c_str());

    if (fid.is_open()) {
        int key;
        int value;
        std::string line;
        std::getline(fid, line); // First line is rows, col
        while (std::getline(fid, line)) {
            std::stringstream linestream(line);
            std::string item;
            std::getline(linestream, item, ',');
            key = stoi(item);
            std::getline(linestream, item, ',');
            value = stoi(item);
            this->put(Key<int>(key), Value<int>(value));
        }
    } else {
        fprintf(stderr, "Unable to read %s\n", file_name.c_str());
        return false;
    }

    return true;

}

template<class T, class U>
bool DB<T, U>::DUMP_MANIFEST(std::string file_path) {
    /*
     * Function DUMP_MANIFEST: Dump our manifest to manifest.leodb
     * Param string: Path to our data directory
     */
    std::string file_name = file_path + "/manifest.leodb";
    std::ifstream possible_file(file_name.c_str());
    if (possible_file.is_open()) {
        std::string new_manifest;
        for(auto pair: manifest){
            // Loop and add each value to string to output
            new_manifest += pair.first + "::" + pair.second + "\n";
        }
        // Overwrite manifest.leodb (i.e. ofstream:trunc)
        std::ofstream overwrite_manifest(file_name, std::ofstream::trunc);
        overwrite_manifest << new_manifest;
        LOG_F(INFO, "Successfully dumped manifest.leodb!");
        possible_file.close();
        overwrite_manifest.close();
    } else {
        LOG_F(ERROR, ("Error dumping manifest!" + file_name + " not found!").c_str());
        return false;
    }
    return true;
}

template<class T, class U>
bool DB<T, U>::LOAD_MANIFEST(std::string file_path) {
    /*
     * Function LOAD_MANIFEST: Load our manifest from a file
     * Param string: Path to our data directory
     */
    std::string file_name = file_path + "/manifest.leodb";
    std::ifstream possible_file(file_name.c_str());
    if (possible_file.is_open()) {
        LOG_F(INFO, "Found manifest.leodb, loading data...");
        std::string output_text;
        while (getline (possible_file, output_text)) {
            std::string variable_name = output_text.substr(0, output_text.find("::"));
            std::string variable_value = output_text.substr( output_text.find("::")+2, output_text.length());
            manifest[variable_name] = variable_value;
        }
        LOG_F(INFO, "Successfully loaded manifest.leodb!");
        possible_file.close();
        return true;
    } else {
        LOG_F(WARNING, "manifest.leodb does not exists, creating manifest.leodb...");
        std::ofstream manifest_file(file_name);
        manifest_file << initialize_manifest();
        manifest_file.close();
        return LOAD_MANIFEST(file_path);
    }
    return false;
}

template<class T, class U>
std::string DB<T, U>::initialize_manifest() {
    /*
     * Function initialize_manifest: Initialize our manifest.leodb file with initial values
     * Return: String to output to file
     */
    return "Height::0\nTierLevels::0\nLevelLevels::0\nCurrentLevel::0\n";
}


template<class T, class U>
bool DB<T, U>::INIT_WRITE_TO_FILE() {
    /*
     * Function WRITE_TO_FILE: Write our in-memory data to disk
     * Return: Bool flag if we were successful
     */
    //TODO - add file header - include number of rows and cols
    std::vector<std::pair<int, Entry<T, U> > > sorted(table.begin(), table.end());

    // Sort and create output string
    std::string output = "";
    std::sort(sorted.begin(), sorted.end(), customLess);
    for (auto pair: sorted) {
        output += (pair.second.buildString() + "\n");
    }

    // Start from level 0 and try to insert
    std::unordered_map<std::string, std::string> root_level = LOAD_LEVEL("0", "Level");

    // TODO: Change this to by dynamic (tier vs level)
    DLOG_F(INFO, "Flushing data to disk...");
    flush_new_level(root_level, sorted);

    // Reset our in-memory values
    table = std::unordered_map<int, Entry<T, U> >();
    totalKeys = 0;
    return true;
}

template<class T, class U>
int DB<T, U>::getHeight() {
    /*
     * Function getHeight: Get the height of flushed memory
     * Return: Integer representing the height
     */
    return  std::stoi(manifest["Height"]);
}

template<class T, class U>
void DB<T, U>::flush_new_level(std::unordered_map<std::string, std::string> level_info,
                                     std::vector<std::pair<int, Entry<T, U>>> sorted) {
    /*
     * Function flush_new_level_level: Flush to a new Tier level
     * Param unordered_map<string, string> level_info: Information on the current level we are on
     * Param vector<pair<int, Entry> > sorted: New sorted run
     */
    int current_run = std::stoi(level_info["CurrentRun"]);
    int max_runs = std::stoi(level_info["MaxRuns"]);

    if (current_run == (max_runs-1)){
        DLOG_F(INFO, ("Level " + level_info["Level"] + " is full").c_str());

        // Get all data from current level -> Push down
        std::vector<std::pair<int, Entry<T, U>>> old_values;
        if (level_info["Type"] == "Tier"){
            old_values = load_tier_data(level_info);
        } else if (level_info["Type"] == "Level") {
            old_values = load_level_data(level_info);
        }

        // Merge old_data with sorted
        for (auto pair: old_values) {
            sorted.push_back(std::pair(pair.first, pair.second));
        }
        std::sort(sorted.begin(), sorted.end(), customLess);

        // Built our output
        std::string output;
        for (auto pair: sorted) {
            output += (pair.second.buildString() + "\n");
        }

        // Delete level content but save level_info.leodb (increment CurrentRun)
        delete_level_content(level_info);

        // Recursively call
        std::string next_level_string = std::to_string(std::stoi(level_info["Level"]) + 1);
        std::unordered_map<std::string, std::string> next_level_info = LOAD_LEVEL(next_level_string, level_info["Type"]);
        return flush_new_level(next_level_info, sorted);

    } else if (current_run < max_runs) {
        DLOG_F(INFO, ("Found free sport at level " + level_info["Level"] + ", current_run: " + level_info["CurrentRun"]).c_str());

        // We have a free spot at this level
        std::sort(sorted.begin(), sorted.end(), customLess);

        // Built our output
        std::string output = "";
        for (auto pair: sorted) {
            output += (pair.second.buildString() + "\n");
        }

        // Add our data to this level
        add_data_to_level(level_info, output);
        return;
    } else {
        LOG_F(ERROR, "This should not happen, oh uh!");
    }
}

template<class T, class U>
void DB<T, U>::delete_level_content(std::unordered_map<std::string, std::string> level_info){
    /*
     * Function delete_level_content: Delete content for a level
     * Param unordered_map<string, string> level_info: Information on the current level we are on
     */
    if (level_info["Type"] == "Tier") {
        // Delete all the runs (i.e. all the folders)
        for (int x = 0; x < std::stoi(level_info["MaxRuns"]); x++) {
            std::string old_folder = DATA_FOLDER_PATH + "/" + level_info["Level"] + "/" + std::to_string(x);
            std::system(("exec rm -r " + old_folder).c_str());
        }
    } else if (level_info["Type"] == "Level") {
        // Delete the one file with the sorted run
        std::string old_data = DATA_FOLDER_PATH + "/" + level_info["Level"] + "/" + level_info["Level"] + ".leodb";
        std::system(("exec rm " + old_data).c_str());
    } else {
        LOG_F(ERROR, "This should not happen, oh uh!");
    }
    level_info["CurrentRun"] = std::to_string(0);
    DUMP_LEVEL(level_info);
}

template<class T, class U>
void DB<T, U>::add_data_to_level(std::unordered_map<std::string, std::string> level_info, std::string output) {
    /*
     * Function add_data_to_level: Add output to level
     * Param unordered_map<string, string> level_info: Information on the current level we are on
     * Param String output: Output we are writing
     */
    if (level_info["Type"] == "Tier"){
        // Write our data
        std::string folder_name = (DATA_FOLDER_PATH + "/" + level_info["Level"] + "/" + level_info["CurrentRun"]);
        std::string file_name = folder_name + "/" + level_info["CurrentRun"] + ".leodb";
        if (!std::filesystem::exists(folder_name.c_str())){
            std::filesystem::create_directory(folder_name);
        }
        // Add the data to the current run
        std::ofstream run_file(file_name);
        run_file << output;
        run_file.close();
    } else if (level_info["Type"] == "Level"){
        // Write to our output
        std::string file_name = (DATA_FOLDER_PATH + "/" + level_info["Level"] + "/" + level_info["Level"] + ".leodb");
        std::ifstream possible_file(file_name);
        std::ofstream out;
        if (possible_file.is_open()) {
            // Append to the end
            out.open(file_name, std::ios::app);
        } else {
            // Create a new file
            std::string run_file_name = file_name;
            out.open(file_name);
        }
        out << output;
        out.close();
        possible_file.close();
    } else {
        LOG_F(ERROR, "This should not happen, oh uh!");
    }
    // Update level_info.leodb
    level_info["CurrentRun"] = std::to_string(std::stoi(level_info["CurrentRun"]) + 1);
    DUMP_LEVEL(level_info);
}

template<class T, class U>
int DB<T, U>::get_lines_in_file(std::string file_path) {
    /*
     * Function get_lines_in_file: Count the number of lines in a file
     * Param String file_path: File we are looking at
     */
    int total_lines = 0;
    std::string s;
    std::ifstream in;
    in.open(file_path);
    while(!in.eof()) {
        getline(in, s);
        total_lines ++;
    }
    in.close();
    return total_lines;
}

template<class T, class U>
std::vector<std::pair<int, Entry<T, U>>> DB<T, U>::load_tier_data(std::unordered_map<std::string, std::string> level_info) {
    /*
     * Function load_tier_data: Load Tier data for level
     * Param unordered_map<string, string> level_info: Information about our current level
     * Return: Vector<pair <int, Entry> > of all the old data
     */
    std::vector<std::pair<int, Entry<T, U> > > ret;

    if (level_info["Type"].compare("Tier") != 0) {
        LOG_F(ERROR, ("Invalid type passed into load_tier_data, type: " + level_info["Type"]).c_str());
        return ret;
    }

    // Loop over runs and collect them all
    DLOG_F(INFO, ("[Tier] Loading level " + level_info["Level"] + " data...").c_str());
    std::string folder_path = DATA_FOLDER_PATH + "/" + level_info["Level"];
    std::string output_text;
    for (int x = 0; x < std::stoi(level_info["MaxRuns"]); x++){
        std::string data_path = folder_path + "/" + std::to_string(x) + "/" + std::to_string(x) + ".leodb";
        std::ifstream possible_file(data_path);
        while (getline(possible_file, output_text)) {
            int hash_key_value = std::stoi(output_text.substr(0, output_text.find("::")));
            std::string key_value = output_text.substr(output_text.find("::") + 2, output_text.find(":::"));
            std::string value_value = output_text.substr(output_text.find(":::") + 3, output_text.length());
            ret.push_back(std::pair<int, Entry<T, U> >(std::pair<int, Entry<T, U> >(hash_key_value, Entry<T, U>(key_value, value_value))));
        }
        possible_file.close();
    }
    return ret;
}

template<class T, class U>
std::vector<std::pair<int, Entry<T, U>>> DB<T, U>::load_level_data(std::unordered_map<std::string, std::string> level_info) {
    /*
     * Function load_level_data: Load Level data for level
     * Param unordered_map<string, string> level_info: Information about our current level
     * Return: Vector<pair <int, Entry> > of all the old data
     */
    // Previous data is sorted, so just load in order
    std::vector<std::pair<int, Entry<T, U> > > ret;

    if (level_info["Type"] != "Level") {
        LOG_F(ERROR, ("Invalid type passed into load_level_data, type: " + level_info["Type"]).c_str());
        return ret;
    }

    // Get path to level data file
    std::ifstream possible_file(DATA_FOLDER_PATH + "/" + level_info["Level"] + "/" + level_info["Level"] + ".leodb");
    if (possible_file.is_open()) {
        DLOG_F(INFO, ("[Level] Loading " + level_info["Level"] + ".leodb...").c_str());

        // Load all values into ret
        std::string output_text;
        while (getline(possible_file, output_text)) {
            int hash_key_value = std::stoi(output_text.substr(0, output_text.find("::")));
            std::string key_value = output_text.substr(output_text.find("::") + 2, output_text.find(":::"));
            std::string value_value = output_text.substr(output_text.find(":::") + 3, output_text.length());
            ret.push_back(std::pair<int, Entry<T, U> >(std::pair<int, Entry<T, U> >(hash_key_value, Entry<T, U>(key_value, value_value))));
        }
        possible_file.close();
        return ret;
    }
    LOG_F(WARNING, ("Level " + level_info["Level"]  + " is empty").c_str());
    return ret;
}


template<class T, class U>
bool DB<T, U>::DUMP_LEVEL(std::unordered_map<std::string, std::string> level_info) {
    /*
     * Function DUMP_LEVEL: Dump level_info map to disk
     * Param unordered_map<string, string> level_info: Map holding all our level info
     * Return: Bool flag to indicate if we were able to update the level info
     */
    std::string level_info_file = DATA_FOLDER_PATH + "/" + level_info["Level"] + "/level_info.leodb";
    std::ifstream possible_file(level_info_file.c_str());
    if (possible_file.is_open()) {
        std::string new_level_info;
        for(auto pair: level_info){
            // Loop and add each value to string to output
            new_level_info += pair.first + "::" + pair.second + "\n";
        }
        // Overwrite level_info.leodb (i.e. ofstream:trunc)
        std::ofstream overwrite_level_info(level_info_file, std::ofstream::trunc);
        overwrite_level_info << new_level_info;
        LOG_F(INFO, ("Successfully dumped level " + level_info["Level"] + "!").c_str());
        possible_file.close();
    } else {
        LOG_F(ERROR, ("Error dumping level " + level_info["Level"]+ "!").c_str());
        return false;
    }
    return true;
}

template<class T, class U>
std::unordered_map<std::string, std::string> DB<T, U>::LOAD_LEVEL(std::string level, std::string type) {
    /*
     * Function LOAD_LEVEL: Load and return level_info.leodb data
     * Param String level: Level we are looking for as a string
     * Param String type: Type of level (Level or Tier)
     * Return: unordered_map<string, string> of relevant information
     */
    std::unordered_map<std::string, std::string> ret;

    // Get path to level data file
    std::string folder_name = (DATA_FOLDER_PATH + "/" + level);
    std::string file_name = (folder_name + "/level_info.leodb");
    std::ifstream possible_file(file_name.c_str());

    if (possible_file.is_open()) {
        DLOG_F(INFO, ("Loading level: " + level).c_str());
        std::string output_text;
        while (getline (possible_file, output_text)) {
            std::string variable_name = output_text.substr(0, output_text.find("::"));
            std::string variable_value = output_text.substr( output_text.find("::")+2, output_text.length());
            ret[variable_name] = variable_value;
        }
        DLOG_F(INFO, ("Successfully loaded level " + level + "!").c_str());
        possible_file.close();
        return ret;
    } else {
        DLOG_F(WARNING, ("Could not find level " + level + ". Creating...").c_str());
        // Create the folder for the level if it doesn't exists
        if (!std::filesystem::exists(folder_name.c_str())){
            DLOG_F(INFO, ("Creating folder for level " + level + "...").c_str());
            std::filesystem::create_directory(folder_name);

            // Update height, store the max level seen so far
            manifest["Height"] = std::to_string(std::max(std::stoi(manifest["Height"]) + 1, std::stoi(manifest["Height"])));
        }
        std::ofstream level_file(file_name);
        int int_level = std::stoi(level);
        int max_runs = std::stoi(MAX_RUNS);

        // Calculate the max_pairs at this level (equal to: ( max_runs^int_level ) * MEMORY_THRESHOLD
        int max_pairs = (int) (std::pow((double) max_runs, (double) int_level) * MEMORY_THRESHOLD);

        level_file << initialize_level(level, type, std::to_string(max_pairs));
        level_file.close();

        return LOAD_LEVEL(level, type);
    }
    LOG_F(ERROR, "This should not happen, oh uh!");
    return ret;
}

template<class T, class U>
std::string DB<T, U>::initialize_level(std::string level, std::string type, std::string max_pairs) {
    /*
     * Function initialize_level: Initialize our level_info.leodb file with initial values
     * Param String level: Level we are looking at
     * Param String type: Type of level (Level or Tier)
     * Param String max_pairs: Max pairs we can hold in each run
     * Return: String to output to file
     */
    return "Level::" + level + "\nCurrentRun::0\nMaxRuns::" + MAX_RUNS + "\nType::" + type + "\nMaxPairs::" + max_pairs;
}

template<class T, class U>
void DB<T, U>::close() {
    /*
     * Function close: Close our DB, saving all relevant information
     */
    LOG_F(WARNING, "Shutting LeoDB down..");

    // Dump our manifest
    DUMP_MANIFEST();

    // Dump our in-memory data
    DUMP_IN_MEMORY();
}

template<class T, class U>
void DB<T, U>::DUMP_IN_MEMORY() {
    /*
     * Function DUMP_IN_MEMORY: Dump our in-memory (table) values
     */
    std::string output = "";
    for (auto pair: table){
        output += (pair.second.buildString() + "\n");
    }
    std::ofstream new_in_memory(DATA_FOLDER_PATH + "/in-memory.leodb", std::ofstream::trunc);
    new_in_memory << output;
    new_in_memory.close();
    LOG_F(INFO, "Successfully dumped in-memory data");
}

template<class T, class U>
void DB<T, U>::LOAD_IN_MEMORY() {
    /*
     * Function LOAD_IN_MEMORY: Load out in-memory.leodb values
     */
    std::ifstream possible_file(DATA_FOLDER_PATH + "/in-memory.leodb");
    if (possible_file.is_open()) {
        DLOG_F(INFO, "Found in-memory.leodb");
        std::string output_text;
        while (getline (possible_file, output_text)) {
            Key<T> key(std::stoi(output_text.substr(output_text.find("::")+2, output_text.find(":::"))));
            Value<U> value(std::stoi(output_text.substr( output_text.find("::")+2, output_text.length())));
            int inserted = key.hashItem();
            if (table.count(inserted) == 0) {
                totalKeys += 1;
            }
            table[inserted] = Entry<T, U>(key, value);
        }
        DLOG_F(INFO, ("Successfully loaded in-memory.leodb! Loaded " + std::to_string(size()) + " values").c_str());
    } else {
        DLOG_F(WARNING, "No in-memory.leodb file could be found");
    }
    possible_file.close();
}



