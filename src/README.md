# LeoDB Source Code
Following is a list of class's along with the functions they implement

## Overview of LeoDB
Following is a high level idea of how data is flushed to memory in LeoDB:

1. Once we have exceeded our `MEMORY_THRESHOLD` we call the function `INIT_WRITE_TO_FILE` to start writing to disk, inside that function we:
  * Sort our in-memory data
  * Call the `flush_new_level` starting with our root level and our sorted data
2. Inside `flush_new_level` we check if we can add another run into the current level:
  * If we *can*:
    * Build a output string with all of our data (i.e. `hash::key:::value::::tombstone`)
    * Call the function `add_data_to_level` with our output string and level_info
  * If we *cannot*:
    * We found a full level!
    * Gather data on the current level (and delete that content on disk)
    * Sort the gathered data with our in-memory data
    * Recursively call `flush_new_level` with our new sorted data
3. Inside `add_data_to_level` we:
  * Consult our monitor class to determine what type of level we should output (**TODO**)
  * Dump our data to disk

## Information on Class's
Following is a list of modules and an explanation of their functions.

### `db`
* Comprised of `db.h` and `db.cpp`, this class is the majority of LeoDB. It holds all the logic and state of our database.
* **Public**
  * **Basic Operators**
    * `bool put (T _key, U _value)`
      * Function: Takes in a Key and Value and inserts into our db
      * Param Key \_key: Key to insert
      * Param Value \_value: Value to insert
      * Return: True/False if it was successful
    * `bool del(T _key)`
      * Function del: Delete a key from our DBMS
      * Param Key \_key: Key to use for lookup
      * Return: True/False if it was successful
    * `Value<T> get(T _key)`
      * Function get: Get a Value from a Key in our DBMS
      * Param Key \_key: Key to use for lookup
      * Return: Value that was found or null if not found
    * `std::vector<Value<T> > scan(Key<T> low, Key<T> high)`
      * Function scan: Get a Value from a Key in our DBMS
      * Param Key low: Lower-bound to scan
      * Param Key high: Higher-bound to scan
      * Return: Vector of values related to Keys between low/high
    * `int min(bool keys=true)`
      * Function min: Get the min value in our DBMS
      * Param bool key: Flag to indicate if we should search the keys or values
      * Return: Int representing the min value
    * `int max(bool keys=true)`
      * Function max: Get the max value in our DBMS
      * Param bool key: Flag to indicate if we should search the keys or values
      * Return: Int representing the max value
    * `float avg(bool keys=true)`
      * Function avg: Get the avg value in our DBMS
      * Param bool key: Flag to indicate if we should search the keys or values
      * Return: Float representing the average of all values
    * `float stddev(bool keys=true)`
      * Function stddev: Get the standard deviation for the data
      * Param bool key: Flag to indicate if we should search the keys or values
      * Return: Float representing the standard deviation of all values
  * **Logistics**
    * `DB_STATUS OPEN_FILE()`
      * CAN BE REMOVED
    * `bool CLOSE()`
      * CAN BE REMOVED
    * `bool LOAD_FROM_FILE(std::string file_name)`
      * CAN BE REMOVED
  * **DB Information**
    * `int size()`
      * Function size: Get the size of the DB
      * Return: Int representing the size
    * `int getHeight()`
      * Function getHeight: Get the height of flushed memory
      * Return: Integer representing the height
    * `void close()`
      * Function close: Close our DB, saving all relevant information

* **Private**
  * **Basic Operators**
    * `bool put (Key<T> _key)`
      * Function put: Helper function for delete, Takes in a Key and makes a tombstone entry
      * Param Key \_key: Key object to insert
      * Return: True/False if it was successful
    * `Value<U> SEARCH_MEMORY(Key<T> key)`
      * Function SEARCH_MEMORY: Searchers memory for a key
      * Param Key key: Key we are looking for
  * **Logistics**
    * `bool DUMP_MANIFEST(std::string file_path=DATA_FOLDER_PATH)`
      * Function DUMP_MANIFEST: Dump our manifest to manifest.leodb
      * Param string: Path to our data directory
    * `bool LOAD_MANIFEST(std::string file_path=DATA_FOLDER_PATH)`
      * Function LOAD_MANIFEST: Load our manifest from a file
      * Param string: Path to our data directory
    * `std::unordered_map<std::string, std::string> LOAD_LEVEL(std::string level, std::string type)`
      * Function LOAD_LEVEL: Load and return level_info.leodb data
      * Param String level: Level we are looking for as a string
      * Param String type: Type of level (Level or Tier)
      * Return: unordered_map<string, string> of relevant information
    * `bool DUMP_LEVEL(std::unordered_map<std::string, std::string> level_info)`
      * Function DUMP_LEVEL: Dump level_info map to disk
      * Param unordered_map<string, string> level_info: Map holding all our level info
      * Return: Bool flag to indicate if we were able to update the level info
    * `bool INIT_WRITE_TO_FILE();`
      * Function WRITE_TO_FILE: Write our in-memory data to disk
      * Return: Bool flag if we were successful
    * `std::string initialize_manifest();`
      * Function initialize_manifest: Initialize our manifest.leodb file with initial values
      * Return: String to output to file
    * `std::string initialize_level(std::string level, std::string type, std::string max_pairs)`
      * Function initialize_level: Initialize our level_info.leodb file with initial values
      * Param String level: Level we are looking at
      * Param String type: Type of level (Level or Tier)
      * Param String max_pairs: Max pairs we can hold in each run
      * Return: String to output to file
    * `void DUMP_IN_MEMORY()`
      * Function DUMP_IN_MEMORY: Dump our in-memory (table) values
    * `void LOAD_IN_MEMORY()`
      * Function LOAD_IN_MEMORY: Load out in-memory.leodb values
  * **Tier vs. Leveling**
    * `std::vector<std::pair<int, Entry<T, U> > > load_level_data(std::unordered_map<std::string, std::string> level_info)`
      * Function load_level_data: Load Level data for level
      * Param unordered_map<string, string> level_info: Information about our current level
      * Return: Vector<pair <int, Entry> > of all the old data
    * `std::vector<std::pair<int, Entry<T, U> > > load_tier_data(std::unordered_map<std::string, std::string> level_info)`
      * Function load_tier_data: Load Tier data for level
      * Param unordered_map<string, string> level_info: Information about our current level
      * Return: Vector<pair <int, Entry> > of all the old data

  * **Flushing**
    * `void flush_new_level(std::unordered_map<std::string, std::string> level_info, std::vector<std::pair<int, Entry<T, U> > > sorted)`
      * Function flush_new_level_level: Flush to a new Tier level
      * Param unordered_map<string, string> level_info: Information on the current level we are on
      * Param vector<pair<int, Entry> > sorted: New sorted run
    * `void add_data_to_level(std::unordered_map<std::string, std::string> level_info, std::string output)`
      * Function add_data_to_level: Add output to level
      * Param unordered_map<string, string> level_info: Information on the current level we are on
      * Param String output: Output we are writing
    * `void delete_level_content(std::unordered_map<std::string, std::string> level_info)`
      * Function delete_level_content: Delete content for a level
      * Param unordered_map<string, string> level_info: Information on the current level we are on
    * `int get_lines_in_file(std::string file_path)`
      * CAN BE REMOVED
