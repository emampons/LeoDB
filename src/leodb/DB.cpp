#include "DB.h"

template<class T>
bool DB<T>::put(Key<T> key, Value value) {
    /*
     * Function put: Takes in a Key and Value and inserts into our DB
     * Param Key key: Key to insert
     * Param Value value: Value to insert
     * Return: True/False if it was successful
     */
    try {
        table[key] = value;
        return true;
    } catch (int e) {
        return false;
    }
}

template<class T>
bool DB<T>::del(Key<T> key) {
    /*
     * Function del: Delete a key from our DBMS
     * Param Key key: Key to use for lookup
     * Return: True/False if it was successful
     */
    try {
        table.erase(key);
        return true;
    } catch (int e) {
        return false;
    }
}

template<class T>
Value DB<T>::get(Key<T> key) {
    /*
     * Function get: Get a Value from a Key in our DBMS
     * Param Key key: Key to use for lookup
     * Return: Value that was found or null if not found
     */
    try {
        return table[key];
    } catch (int e) {
        return null;
    }
}

template<class T>
std::vector<Value> DB<T>::scan(Key<T> low, Key<T> high) {
    /*
     * Function scan: Get a Value from a Key in our DBMS
     * Param Key low: Lower-bound to scan
     * Param Key high: Higher-bound to scan
     * Return: Vector of values related to Keys between low/high
     */
    return std::vector<Value>();
}

template<class T>
int DB<T>::min(){
    return -1;
}

template<class T>
int DB<T>::max(){
    return -1;
}

template<class T>
float DB<T>::avg(){
    return -1;
}

template<class T>
float DB<T>::stddev(){
    return -1;
}
