#include <cmath>
#include <db.h>

template<class T, class U>
bool DB<T, U>::put(Key<T> key, Value<U> value) {
    /*
     * Function put: Takes in a Key and Value and inserts into our db
     * Param Key key: Key to insert
     * Param Value value: Value to insert
     * Return: True/False if it was successful
     */
    int inserted = key.hashItem();
    if (table.count(inserted) == 0) {
        totalKeys += 1;
    }
    table[inserted] = Entry<T, U>(key, value);

    if (totalKeys > MEMORY_THRESHOLD) {
        WRITE_TO_FILE();
    }
    return true;
}


template<class T, class U>
bool DB<T, U>::del(Key<T> key) {
    /*
     * Function del: Delete a key from our DBMS
     * Param Key key: Key to use for lookup
     * Return: True/False if it was successful
     */
    bool ret = table.erase(key.hashItem());
    if (ret) {
        totalKeys -= 1;
    }
    return ret;
}

template<class T, class U>
Value<T> DB<T, U>::get(Key<T> key) {
    /*
     * Function get: Get a Value from a Key in our DBMS
     * Param Key key: Key to use for lookup
     * Return: Value that was found or null if not found
     */
    try {
        return table[key.hashItem()].getValue();
    } catch (int e) {
        return Value<T>();
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
        //std::cout<<entry.getKey()<<"\n";
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
bool DB<T, U>::CLOSE() {
    OPEN_FILE();
    if (file.is_open()) {
        WRITE_TO_FILE();
        file.close();
    }

    db_status = CLOSED;
}

template<class T, class U>
bool DB<T, U>::LOAD_FROM_FILE(const std::string& file_name) {
    std::ifstream fid(file_name);

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
bool DB<T, U>::WRITE_TO_FILE() {
    file.clear();
    //TODO - add file header - include number of rows and cols
    struct {
        // Helper function to compare pairs
        bool operator()(const std::pair<int, Entry<T, U> > &a, const std::pair<int, Entry<T, U> > &b) const {
            return a.first < b.first;
        }
    } customLess;

    //std::ofstream outfile (std::to_string(4) + ".txt", std::ios_base::app);
    std::vector<std::pair<int, Entry<T, U> > > sorted(table.begin(), table.end());
    std::sort(sorted.begin(), sorted.end(), customLess);

    for (auto pair: sorted) {
        file << pair.second.getKey() <<" ," <<pair.second.getValue().getItem()<< "\n";
    }

    // Reset our in-memory values
    table = std::unordered_map<int, Entry<T, U> >();
    totalKeys = 0;
    return true;
}