#include "db.h"

template<class T>
bool DB::put(Key<T> key, Value<T> value) {
    /*
     * Function put: Takes in a Key and Value and inserts into our db
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
bool DB::del(Key<T> key) {
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
Value<T>  DB::get(Key<T> key) {
    /*
     * Function get: Get a Value from a Key in our DBMS
     * Param Key key: Key to use for lookup
     * Return: Value that was found or null if not found
     */
    try {
        return table[key];
    } catch (int e) {
        return Value<T>();
    }
}

//template<class T>
//std::vector<Value<T> > DB::scan(Key<T> low, Key<T> high) {
//    /*
//     * Function scan: Get a Value from a Key in our DBMS
//     * Param Key low: Lower-bound to scan
//     * Param Key high: Higher-bound to scan
//     * Return: Vector of values related to Keys between low/high
//     */
//    return std::vector<Value<T> >();
//}
//
//template<class T>
//int DB::min(){
//    return -1;
//}
//
//template<class T>
//int DB<T>::max(){
//    return -1;
//}
//
//template<class T>
//float DB<T>::avg(){
//    return -1;
//}
//
//template<class T>
//float DB<T>::stddev(){
//    return -1;
//}
