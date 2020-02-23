#include "db.h"
#include "entry.h"

template<class T, class U>
bool DB<T, U>::put(Key<T> key, Value<U> value) {
    /*
     * Function put: Takes in a Key and Value and inserts into our db
     * Param Key key: Key to insert
     * Param Value value: Value to insert
     * Return: True/False if it was successful
     */
    try {
        int inserted = key.hashItem();
        table[inserted] = Entry<T, U>(key, value);
        //std::cout<<table[inserted].getKey().getI();
        return true;
    } catch (int e) {
        return false;
    }
}


template<class T, class U>
bool DB<T, U>::del(Key<T> key) {
    /*
     * Function del: Delete a key from our DBMS
     * Param Key key: Key to use for lookup
     * Return: True/False if it was successful
     */
    try {
        return table.erase(key.hashItem());
    } catch (int e) {
        return false;
    }
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

    for (auto pair: table)
    {
        Entry<T, U> entry = pair.second;
        //std::cout<<entry.getKey()<<"\n";
        if ((entry.getKey() >= low) && (entry.getKey() <= high))
            ret.push_back(entry.getValue());
    }
    return  ret;
}

template<class T, class U>
int DB<T, U>::min(bool keys){
    int min = INT_MAX;
    if(keys){
        for(auto pair: table){
            Entry<T, U> entry = pair.second;
            if(entry.getKey() < min){
                min = entry.getKey();
            }
        }

    }else{
        for(auto pair: table){
            Entry<T, U> entry = pair.second;
            if(entry.getValue() < min){
                min = entry.getValue();
            }
        }
    }

    return min;
}

template<class T, class U>
int DB<T, U>::max(bool keys){
    int max = INT_MIN;
    if(keys){
        for(auto pair: table){
            Entry<T, U> entry = pair.second;
            if(entry.getKey() > max){
                max = entry.getKey();
            }
        }

    }else{
        for(auto pair: table){
            Entry<T, U> entry = pair.second;
            if(entry.getValue() > max){
                max = entry.getValue();
            }
        }
    }

    return max;
}
//
template<class T, class U>
float DB<T, U>::avg(bool keys){
    /*
     * Function avg: Get the avg value in our DBMS
     * Param bool key: Flag to indicate if we should search the keys or values
     * Return: Float representing the average of all values
     */
    int number_of_entries = 0;
    float sum = 0.0;
    typename std::unordered_map<int, Entry<T, U> >::iterator iter = table.begin();
    while(iter != table.end()){
        if (keys){
            sum += iter->second.getKey().getItem();
        } else {
            sum += iter->second.getValue().getItem();
        }
    }
    return (sum/number_of_entries);
}
//
//template<class T>
//float DB<T>::stddev(){
//    return -1;
//}
