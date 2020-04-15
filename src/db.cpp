#include <cmath>
#include <db.h>

template<class T, class U>
DB<T, U>::DB(std::string starting_level, std::string _file_path, std::string logger_path) {
    /*
     * Function constructor: Start up our DB
     */
    // Load our logger
    loguru::add_file(logger_path.c_str(), loguru::Append, loguru::Verbosity_MAX);
    LOG_F(INFO, "Logger Loaded - Starting LeoDB!");

    // Initialize local variables
    totalKeys = 0;
    MEMORY_THRESHOLD = IN_MEMORY_THRESHOLD;
    monitor.set_starting_level(starting_level);
    monitor.set_tune(true);
    file_path = _file_path;

    // Load our Manifest file
    LOG_F(INFO, "Attempting to load manifest file...");
    LOAD_MANIFEST();

    // Load our in-memory data
    LOG_F(INFO, "Attempting to load previous in-memory data...");
    LOAD_IN_MEMORY();
}

template<class T, class U>
void DB<T, U>::set_tune(bool _tune) {
    /*
     * Function set_tune: Helper function to disable/enable tuning
     * Param bool _tune: True/False if we should enable/disable turning
     */
    monitor.set_tune(_tune);
}

template<class T, class U>
bool DB<T, U>::put(T _key, U _value) {
    /*
     * Function put: Takes in a Key and Value and inserts into our db
     * Param Key _key: Key to insert
     * Param Value _value: Value to insert
     * Return: True/False if it was successful
     */
    monitor.monitor("WRITE");
    Key<T> key(_key);
    Value<U> value(_value);
    int inserted = key.hashItem();
    if (table.count(inserted) == 0) {
        totalKeys += 1;
    }
    Entry<T, U> insert(key, value);

    table[inserted] = insert;
    DLOG_F(INFO, ("Added new Key/Value pair, Hash::Key:::Value->" + table[inserted].buildString()).c_str());


    if (totalKeys >= MEMORY_THRESHOLD) {
        INIT_WRITE_TO_FILE();
    }
    return true;
}

template<class T, class U>
bool DB<T, U>::put(Key<T> _key) {
    /*
     * Function put: Helper function for delete, Takes in a Key and makes a tombstone entry
     * Param Key _key: Key object to insert
     * Return: True/False if it was successful
     */
    // Create a Tombstone entry
    int inserted = _key.hashItem();
    Entry<T, U> insert(_key, Value<U>());
    insert.tomb_it();
    if (table.count(inserted) == 0) {
        totalKeys += 1;
    }

    // Insert the new Tombstone entry
    table[inserted] = insert;

    // Flush if we need too
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
        // Insert the tombstone into the in-memory table
        Entry<T, U> new_value (key, Value<U>(true));
        new_value.tomb_it();
        table[key.hashItem()] = new_value;
    } else {
        // Else insert the special deleted Entry
        DLOG_F(INFO, ("Inserting Tombstone for Key: " + key.getString()).c_str());
        put(key);
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
    monitor.monitor("READ");
    DLOG_F(INFO, ("Getting " + std::to_string(_key)).c_str());
    Key<T> key(_key);
    if (table.count(key.hashItem())) {
        // Return it from the in-memory table
        DLOG_F(INFO, "Found key in-memory!");
        Entry<T, U> ret = table[key.hashItem()];
        if (ret.is_tomb()){
            return Value<U>(true);
        } else {
            return ret.getValue();
        }
    } else {
//        bool found = false;
//        for (auto &x : bloom_filter) {
//            if (x.second.query(key.getItem()))
//                found = true;
//        }
//        if (found) {
//            DLOG_F(INFO, "Found key in bloom filter");
//            Entry<T, U> ret;
//            for (auto &x : fence_pointer) {
//                ret = x.second.search(key);
//            }
//            if (ret.getKey().hashItem() == key.hashItem()) {
//                DLOG_F(INFO, "Found key in fence pointer!");
//                return ret.getValue();
//            }
//        } else {
            DLOG_F(INFO, "Searching memory...");
            return SEARCH_MEMORY(key);
//        }

    }
    DLOG_F(WARNING, "Uh Oh! This should not happen!");
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
        // Check in-memory first
        Entry<T, U> entry = pair.second;
        if ((entry.getKey() >= low) && (entry.getKey() <= high))
            ret.push_back(entry.getValue());
    }
    for (int x = 0; x < std::stoi(manifest["Height"]); x++) {
        std::unordered_map<std::string, std::string> current_level = LOAD_LEVEL(std::to_string(x), "level");
        std::vector<std::pair<int, Entry<T, U>>> values;
        if (current_level["Type"] == "tier") {
            values = load_tier_data(current_level);
            for (auto pair: values) {
                if ((pair.second.getKey() >= low) && (pair.second.getKey() <= high))
                    ret.push_back(pair.second.getValue());
            }
        } else if (current_level["Type"] == "level") {
            values = load_level_data(current_level);
            for (auto pair: values) {
                if ((pair.second.getKey() >= low) && (pair.second.getKey() <= high))
                    ret.push_back(pair.second.getValue());
            }
        }
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
    for (auto pair: table) {
        // Check in-memory first
        Entry<T, U> entry = pair.second;
        if (keys) {
            if (entry.getKey().getItem() < min) {
                min = entry.getKey().getItem();
            }
        } else {
            if (entry.getValue().getItem() < min) {
                min = entry.getValue().getItem();
            }
        }
    }
    for (int x = 0; x < std::stoi(manifest["Height"]); x++) {
        std::unordered_map<std::string, std::string> current_level = LOAD_LEVEL(std::to_string(x), "level");
        std::vector<std::pair<int, Entry<T, U>>> values;
        if (current_level["Type"] == "tier") {
            values = load_tier_data(current_level);
            for (auto pair: values) {
                if (keys) {
                    if (pair.second.getKey().getItem() < min) {
                        min = pair.second.getKey().getItem();
                    }
                } else {
                    if (pair.second.getValue().getItem() < min) {
                        min = pair.second.getValue().getItem();
                    }
                }
            }
        } else if (current_level["Type"] == "level") {
            values = load_level_data(current_level);
            for (auto pair: values) {
                if (keys) {
                    if (pair.second.getKey().getItem() < min) {
                        min = pair.second.getKey().getItem();
                    }
                } else {
                    if (pair.second.getValue().getItem() < min) {
                        min = pair.second.getValue().getItem();
                    }
                }
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
    for (auto pair: table) {
        // Check in-memory first
        Entry<T, U> entry = pair.second;
        if (keys) {
            if (entry.getKey().getItem() > max) {
                max = entry.getKey().getItem();
            }
        } else {
            if (entry.getValue().getItem() > max) {
                max = entry.getValue().getItem();
            }
        }
    }
    for (int x = 0; x < std::stoi(manifest["Height"]); x++) {
        std::unordered_map<std::string, std::string> current_level = LOAD_LEVEL(std::to_string(x), "level");
        std::vector<std::pair<int, Entry<T, U>>> values;
        if (current_level["Type"] == "tier") {
            values = load_tier_data(current_level);
            for (auto pair: values) {
                if (keys) {
                    if (pair.second.getKey().getItem() > max) {
                        max = pair.second.getKey().getItem();
                    }
                } else {
                    if (pair.second.getValue().getItem() > max) {
                        max = pair.second.getValue().getItem();
                    }
                }
            }
        } else if (current_level["Type"] == "level") {
            values = load_level_data(current_level);
            for (auto pair: values) {
                if (keys) {
                    if (pair.second.getKey().getItem() > max) {
                        max = pair.second.getKey().getItem();
                    }
                } else {
                    if (pair.second.getValue().getItem() > max) {
                        max = pair.second.getValue().getItem();
                    }
                }
            }
        }
    }

    return max;
}


template<class T, class U>
float DB<T, U>::avg(bool keys) {
    /*
     * Function avg: Get the avg value in our DBMS
     * Param bool key: Flag to indicate if we should search the keys or values
     * Return: Float representing the average of all values
     */
    float running_sum = 0.0;
    float total_keys = 0.0;
    for (auto pair: table) {
        // Check in-memory first
        Entry<T, U> entry = pair.second;
        if (keys) {
            running_sum += entry.getKey().getItem();
            total_keys += 1;
        } else {
            running_sum += entry.getValue().getItem();
            total_keys += 1;
        }
    }
    for (int x = 0; x < std::stoi(manifest["Height"]); x++) {
        std::unordered_map<std::string, std::string> current_level = LOAD_LEVEL(std::to_string(x), "level");
        std::vector<std::pair<int, Entry<T, U>>> values;
        if (current_level["Type"] == "tier") {
            values = load_tier_data(current_level);
            for (auto pair: values) {
                if (keys) {
                    running_sum += pair.second.getKey().getItem();
                    total_keys += 1;
                } else {
                    running_sum += pair.second.getValue().getItem();
                    total_keys += 1;
                }
            }
        } else if (current_level["Type"] == "level") {
            values = load_level_data(current_level);
            for (auto pair: values) {
                if (keys) {
                    running_sum += pair.second.getKey().getItem();
                    total_keys += 1;
                } else {
                    running_sum += pair.second.getValue().getItem();
                    total_keys += 1;
                }
                }
            }
    }
    return (running_sum / total_keys );
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
    float total_keys = 0.0;
    float running_sum = 0.0;

    // Loop over and sum the difference squared
    for (auto pair: table) {
        // Check in-memory first
        Entry<T, U> entry = pair.second;
        if (keys) {
            running_sum += pow((entry.getKey().getItem() - average), 2);
            total_keys += 1;
        } else {
            running_sum += pow((entry.getKey().getItem() - average), 2);
            total_keys += 1;
        }
    }
    for (int x = 0; x < std::stoi(manifest["Height"]); x++) {
        std::unordered_map<std::string, std::string> current_level = LOAD_LEVEL(std::to_string(x), "level");
        std::vector<std::pair<int, Entry<T, U>>> values;
        if (current_level["Type"] == "tier") {
            values = load_tier_data(current_level);
            for (auto pair: values) {
                if (keys) {
                    running_sum += pow((pair.second.getKey().getItem() - average), 2);
                    total_keys += 1;
                } else {
                    running_sum += pow((pair.second.getKey().getItem() - average), 2);
                    total_keys += 1;
                }
            }
        } else if (current_level["Type"] == "level") {
            values = load_level_data(current_level);
            for (auto pair: values) {
                if (keys) {
                    running_sum += pow((pair.second.getKey().getItem() - average), 2);
                    total_keys += 1;
                } else {
                    running_sum += pow((pair.second.getKey().getItem() - average), 2);
                    total_keys += 1;
                }
            }
        }
    }
    return sqrt(running_sum / total_keys);
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
Value<U> DB<T, U>::SEARCH_MEMORY(Key<T> key) {
    /*
     * Function SEARCH_MEMORY: Searchers memory for a key
     * Param Key key: Key we are looking for
     */
    // Hash our key
    int key_hash = key.hashItem();

    // Loop over all levels until we find the hash
    for (int x = 0; x < std::stoi(manifest["Height"]); x++) {
        std::unordered_map<std::string, std::string> current_level = LOAD_LEVEL(std::to_string(x), "level");
        std::vector<std::pair<int, Entry<T, U>>> values;
        if (current_level["Type"] == "tier") {
            values = load_tier_data(current_level);
            for (auto pair: values) {
                if (pair.first == key_hash) {
                    // First update our level metadata
                    current_level["LevelReads"] = std::to_string(std::stoi(current_level["LevelReads"]) + 1);
                    DUMP_LEVEL(current_level);

                    // Return our value
                    return pair.second.getValue();
                }
            }
        } else if (current_level["Type"] == "level") {
            values = load_level_data(current_level);
            if (values.size() > 0) {
                Value<U> possible_value = binary_search(values, 0, values.size(), key_hash);
                if (!possible_value.getTomb()) {
                    return possible_value;
                }
            }
        }
    }
    LOG_F(WARNING, ("No value found for key " + key.getString()).c_str());
    return Value<U>(true);
}

template<class T, class U>
Value<U> DB<T, U>::binary_search(std::vector<std::pair<int, Entry<T, U>>> values, int start, int end, int key_hash) {
    /*
     * Function binary_search: Binary search function
     * Param pair<int, Entry> values: Values we are looking at
     * Param int start: Start index
     * Param int end: End index
     * Param int key_hash: Key hash we are looking for
     */
    if (end >= start){
        int mid = start + ((end - start) / 2);
        if (values[mid].first == key_hash){
            return values[mid].second.getValue();
        }
        if (values[mid].first > key_hash){
            return binary_search(values, start, mid-1, key_hash);
        }
        if (values[mid].first < key_hash){
            return binary_search(values, mid+1, end, key_hash);
        }

    }
    return Value<U>(true);
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
bool DB<T, U>::DUMP_MANIFEST() {
    /*
     * Function DUMP_MANIFEST: Dump our manifest to manifest.leodb
     * Param string: Path to our data directory
     */
    std::string file_name = file_path + "/manifest.leodb";
    std::ifstream possible_file(file_name.c_str());
    if (possible_file.is_open()) {
        std::string new_manifest;
        for (auto pair: manifest) {
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
bool DB<T, U>::LOAD_MANIFEST() {
    /*
     * Function LOAD_MANIFEST: Load our manifest from a file
     */
    std::string file_name = file_path + "/manifest.leodb";
    if (!std::filesystem::exists(file_path.c_str())) {
        std::filesystem::create_directory(file_path.c_str());
    }
    std::ifstream possible_file(file_name.c_str());
    if (possible_file.is_open()) {
        LOG_F(INFO, "Found manifest.leodb, loading data...");
        std::string output_text;
        while (getline(possible_file, output_text)) {
            std::string variable_name = output_text.substr(0, output_text.find("::"));
            std::string variable_value = output_text.substr(output_text.find("::") + 2, output_text.length());
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
        return LOAD_MANIFEST();
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
    std::vector<std::pair<int, Entry<T, U> > > sorted(table.begin(), table.end());

    // Start from level 0 and try to insert
    std::unordered_map<std::string, std::string> root_level = LOAD_LEVEL("0", monitor.get_starting_level());
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
    return std::stoi(manifest["Height"]);
}

template<class T, class U>
void DB<T, U>::flush_new_level(std::unordered_map<std::string, std::string> level_info,
                               std::vector<std::pair<int, Entry<T, U>>> sorted) {
    /*
     * Function flush_new_level_level: Flush to a new level
     * Param unordered_map<string, string> level_info: Information on the current level we are on
     * Param vector<pair<int, Entry> > sorted: New sorted run
     */
    int current_run = std::stoi(level_info["CurrentRun"]);
    int max_runs = std::stoi(level_info["MaxRuns"]);

    if (current_run == (max_runs - 1)) {
        DLOG_F(INFO, ("level " + level_info["level"] + " is full").c_str());
        //remove all teirs since it's going to become a new level soon
//        for(int i=0; i<max_runs; i++){
//            std::string to_remove = level_info["level"]+"."+std::to_string(i);
//            fence_pointer.erase(to_remove);
//            bloom_filter.erase(to_remove);
//        }
        // Get all data from current level -> Push down
        std::vector<std::pair<int, Entry<T, U>>> old_values;
        if (level_info["Type"] == "tier") {
            old_values = load_tier_data(level_info);
        } else if (level_info["Type"] == "level") {
            old_values = load_level_data(level_info);
        }

        // Merge old_data with sorted
        for (auto pair: old_values) {
            sorted.push_back(std::pair(pair.first, pair.second));
        }

        // Delete level content but save level_info.leodb (increment CurrentRun)
        DLOG_F(INFO, ("Deleting level " + level_info["level"]).c_str());
        delete_level_content(level_info);

        // Recursively call
        std::string next_level_string = std::to_string(std::stoi(level_info["level"]) + 1);
        std::unordered_map<std::string, std::string> next_level_info = LOAD_LEVEL(next_level_string,
                                                                                  level_info["Type"]);
        return flush_new_level(next_level_info, sorted);

    } else if (current_run < max_runs) {
        // We have a free spot at this level
//        FencePointer<U, T> temp_fence;
//        BloomFilter temp_bloom;
        // Built our output
//        std::string output = "";
        std::string file_name;
        if (level_info["Type"] == "tier") {
            file_name = DATA_FOLDER_PATH + "/" + level_info["level"] + "/" + std::to_string(current_run) + "/" +
                        std::to_string(current_run) + ".leodb";
        } else if (level_info["Type"] == "level") {
            file_name = DATA_FOLDER_PATH + "/" + level_info["level"] + "/" + level_info["level"] + ".leodb";

        }

//        std::string insert_string = level_info["level"] + "." + std::to_string(current_run);
//        bloom_filter[insert_string] = temp_bloom;
//        fence_pointer[insert_string] = temp_fence;

        DLOG_F(INFO, ("Found free spot at level " + level_info["level"] + ", current_run: " + level_info["CurrentRun"]).c_str());

        // Add our data to this level
        add_data_to_level(level_info, sorted);

        return;
    } else {
        LOG_F(ERROR, "This should not happen, oh uh!");
    }
}

template<class T, class U>
void DB<T, U>::delete_level_content(std::unordered_map<std::string, std::string> level_info) {
    /*
     * Function delete_level_content: Delete content for a level
     * Param unordered_map<string, string> level_info: Information on the current level we are on
     */
    if (level_info["Type"] == "tier") {
        // Delete all the runs (i.e. all the content inside the folders)
        for (int x = 0; x < std::stoi(level_info["MaxRuns"]); x++) {
            std::string old_folder = DATA_FOLDER_PATH + "/" + level_info["level"] + "/" + std::to_string(x);
            std::system(("exec rm -r " + old_folder + "/").c_str());
        }
    } else if (level_info["Type"] == "level") {
        // Delete the one file with the sorted run
        std::string old_data = DATA_FOLDER_PATH + "/" + level_info["level"] + "/" + level_info["level"] + ".leodb";
        std::system(("exec rm " + old_data).c_str());
    } else {
        LOG_F(ERROR, "This should not happen, oh uh!");
    }
    level_info["CurrentRun"] = std::to_string(0);
    DUMP_LEVEL(level_info);
}


template<class T, class U>
void DB<T, U>::add_data_to_level(std::unordered_map<std::string, std::string> level_info, std::vector<std::pair<int, Entry<T, U> > > sorted) {
    /*
     * Function add_data_to_level: Add output to level
     * Param unordered_map<string, string> level_info: Information on the current level we are on
     * Param unordered_map<int, Entry> sorted: Our sorted entries
     */
    // As our monitor what we should make the next level
    level_info = monitor.optimize(level_info);

    if (level_info["Type"] == "tier") {
        // Sort
        std::sort(sorted.begin(), sorted.end(), customLess);

        // Build output
        std::string output = "";
        for (auto pair: sorted) {
//            int key = pair.second.getKey().getItem();
//            temp_bloom.program(key);
//            temp_fence.update_pointer(pair.second, file_name);
            output += (pair.second.buildString() + "\n");
        }

        // Write our data
        std::string folder_name = (DATA_FOLDER_PATH + "/" + level_info["level"] + "/" + level_info["CurrentRun"]);
        std::string file_name = folder_name + "/" + level_info["CurrentRun"] + ".leodb";
        if (!std::filesystem::exists(folder_name.c_str())) {
            std::filesystem::create_directory(folder_name);
        }

        // Add the data to the current run
        std::ofstream run_file(file_name);
        run_file << output;
        run_file.close();
    } else if (level_info["Type"] == "level") {
        // Write to our output
        std::string file_name = (DATA_FOLDER_PATH + "/" + level_info["level"] + "/" + level_info["level"] + ".leodb");
        std::ifstream possible_file(file_name);
        std::string output = "";

        if (possible_file.is_open()) {
            // Load in all the values if they are there
            std::string output_text;
            while (getline (possible_file, output_text)) {
                int hash_key_value = std::stoi(output_text.substr(0, output_text.find("::")));
                std::string key_value = output_text.substr(output_text.find("::") + 2, output_text.find(":::"));
                std::string value_value = output_text.substr(output_text.find(":::") + 3, output_text.find("::::"));
                std::string deleted = output_text.substr(output_text.find("::::") + 4, output_text.find("::::"));
                Entry<T, U> new_entry(key_value, value_value);
                if (deleted.compare("1") != 0) {
                    sorted.push_back(std::pair<int, Entry<T, U> >(
                            std::pair<int, Entry<T, U> >(hash_key_value, new_entry)));
                } else {
                    new_entry.tomb_it();
                    sorted.push_back(std::pair<int, Entry<T, U> >(
                            std::pair<int, Entry<T, U> >(hash_key_value, new_entry)));
                }
            }

            // Sort
            std::sort(sorted.begin(), sorted.end(), customLess);

            // Create output
            for (auto pair: sorted) {
//                int key = pair.second.getKey().getItem();
//            temp_bloom.program(key);
//            temp_fence.update_pointer(pair.second, file_name);
                output += (pair.second.buildString() + "\n");
            }
        } else {
            // Sort
            std::sort(sorted.begin(), sorted.end(), customLess);

            // Create output
            for (auto pair: sorted) {
//                int key = pair.second.getKey().getItem();
//            temp_bloom.program(key);
//            temp_fence.update_pointer(pair.second, file_name);
                output += (pair.second.buildString() + "\n");
            }
        }

        // Write the file
        std::ofstream out;
        out.open(file_name, std::ofstream::trunc);

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
    while (!in.eof()) {
        getline(in, s);
        total_lines++;
    }
    in.close();
    return total_lines;
}

template<class T, class U>
std::vector<std::pair<int, Entry<T, U>>>
DB<T, U>::load_tier_data(std::unordered_map<std::string, std::string> level_info) {
    /*
     * Function load_tier_data: Load tier data for level
     * Param unordered_map<string, string> level_info: Information about our current level
     * Return: Vector<pair <int, Entry> > of all the old data
     */
    std::vector<std::pair<int, Entry<T, U> > > ret;

    if (level_info["Type"].compare("tier") != 0) {
        LOG_F(ERROR, ("Invalid type passed into load_tier_data, type: " + level_info["Type"]).c_str());
        return ret;
    }

    // Loop over runs and collect them all
    DLOG_F(INFO, ("[tier] Loading level " + level_info["level"] + " data...").c_str());
    std::string folder_path = DATA_FOLDER_PATH + "/" + level_info["level"];
    std::string output_text;
    for (int x = 0; x < std::stoi(level_info["MaxRuns"]); x++) {
        std::string data_path = folder_path + "/" + std::to_string(x) + "/" + std::to_string(x) + ".leodb";
        std::ifstream possible_file(data_path);
        while (getline(possible_file, output_text)) {
            int hash_key_value = std::stoi(output_text.substr(0, output_text.find("::")));
            std::string key_value = output_text.substr(output_text.find("::") + 2, output_text.find(":::"));
            std::string value_value = output_text.substr(output_text.find(":::") + 3, output_text.find("::::"));
            std::string deleted = output_text.substr(output_text.find("::::") + 4, output_text.find("::::"));
            Entry<T, U> new_entry(key_value, value_value);
            if (deleted.compare("1") != 0) {
                ret.push_back(std::pair<int, Entry<T, U> >(
                        std::pair<int, Entry<T, U> >(hash_key_value, new_entry)));
            } else {
                new_entry.tomb_it();
                ret.push_back(std::pair<int, Entry<T, U> >(
                        std::pair<int, Entry<T, U> >(hash_key_value, new_entry)));
            }
        }
        possible_file.close();
    }
    return ret;
}

template<class T, class U>
std::vector<std::pair<int, Entry<T, U>>>
DB<T, U>::load_level_data(std::unordered_map<std::string, std::string> level_info) {
    /*
     * Function load_level_data: Load level data for level
     * Param unordered_map<string, string> level_info: Information about our current level
     * Return: Vector<pair <int, Entry> > of all the old data
     */
    // Previous data is sorted, so just load in order
    std::vector<std::pair<int, Entry<T, U> > > ret;

    if (level_info["Type"] != "level") {
        LOG_F(ERROR, ("Invalid type passed into load_level_data, type: " + level_info["Type"]).c_str());
        return ret;
    }

    // Get path to level data file
    std::ifstream possible_file(DATA_FOLDER_PATH + "/" + level_info["level"] + "/" + level_info["level"] + ".leodb");
    if (possible_file.is_open()) {
        DLOG_F(INFO, ("[level] Loading " + level_info["level"] + ".leodb...").c_str());

        // Load all values into ret
        std::string output_text;
        while (getline(possible_file, output_text)) {
            int hash_key_value = std::stoi(output_text.substr(0, output_text.find("::")));
            std::string key_value = output_text.substr(output_text.find("::") + 2, output_text.find(":::"));
            std::string value_value = output_text.substr(output_text.find(":::") + 3, output_text.find("::::"));
            std::string deleted = output_text.substr(output_text.find("::::") + 4, output_text.find("::::"));
            Entry<T, U> new_entry(key_value, value_value);
            if (deleted.compare("1") != 0) {
                ret.push_back(std::pair<int, Entry<T, U> >(
                        std::pair<int, Entry<T, U> >(hash_key_value, new_entry)));
            } else {
                new_entry.tomb_it();
                ret.push_back(std::pair<int, Entry<T, U> >(
                        std::pair<int, Entry<T, U> >(hash_key_value, new_entry)));
            }
        }
        possible_file.close();
        return ret;
    }
    LOG_F(WARNING, ("level " + level_info["level"] + " is empty").c_str());
    return ret;
}


template<class T, class U>
bool DB<T, U>::DUMP_LEVEL(std::unordered_map<std::string, std::string> level_info) {
    /*
     * Function DUMP_LEVEL: Dump level_info map to disk
     * Param unordered_map<string, string> level_info: Map holding all our level info
     * Return: Bool flag to indicate if we were able to update the level info
     */
    std::string level_info_file = DATA_FOLDER_PATH + "/" + level_info["level"] + "/level_info.leodb";
    std::ifstream possible_file(level_info_file.c_str());
    if (possible_file.is_open()) {
        std::string new_level_info;
        for (auto pair: level_info) {
            // Loop and add each value to string to output
            new_level_info += pair.first + "::" + pair.second + "\n";
        }
        // Overwrite level_info.leodb (i.e. ofstream:trunc)
        std::ofstream overwrite_level_info(level_info_file, std::ofstream::trunc);
        overwrite_level_info << new_level_info;
        LOG_F(INFO, ("Successfully dumped level " + level_info["level"] + "!").c_str());
        possible_file.close();
    } else {
        LOG_F(ERROR, ("Error dumping level " + level_info["level"] + "!").c_str());
        return false;
    }
    return true;
}

template<class T, class U>
std::unordered_map<std::string, std::string> DB<T, U>::LOAD_LEVEL(std::string level, std::string type) {
    /*
     * Function LOAD_LEVEL: Load and return level_info.leodb data
     * Param String level: level we are looking for as a string
     * Param String type: Type of level (level or tier)
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
        while (getline(possible_file, output_text)) {
            std::string variable_name = output_text.substr(0, output_text.find("::"));
            std::string variable_value = output_text.substr(output_text.find("::") + 2, output_text.length());
            ret[variable_name] = variable_value;
        }
        DLOG_F(INFO, ("Successfully loaded level " + level + "!").c_str());
        possible_file.close();
        return ret;
    } else {
        DLOG_F(WARNING, ("Could not find level " + level + ". Creating...").c_str());
        // Create the folder for the level if it doesn't exists
        if (!std::filesystem::exists(folder_name.c_str())) {
            DLOG_F(INFO, ("Creating folder for level " + level + "...").c_str());
            std::filesystem::create_directory(folder_name);

            // Update height, store the max level seen so far
            manifest["Height"] = std::to_string(
                    std::max(std::stoi(manifest["Height"]) + 1, std::stoi(manifest["Height"])));
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
     * Param String level: level we are looking at
     * Param String type: Type of level (level or tier)
     * Param String max_pairs: Max pairs we can hold in each run
     * Return: String to output to file
     */
    return "level::" + level +
           "\nCurrentRun::0" +
           "\nMaxRuns::" + MAX_RUNS +
           "\nType::" + type +
           "\nMaxPairs::" + max_pairs +
           "\nLevelReads::0";
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
    for (auto pair: table) {
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
        while (getline(possible_file, output_text)) {
            Key<T> key(std::stoi(output_text.substr(output_text.find("::") + 2, output_text.find(":::"))));
            Value<U> value(std::stoi(output_text.substr(output_text.find("::") + 2, output_text.length())));
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
