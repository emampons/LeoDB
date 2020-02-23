#include "DB.h"

// Basic Operators Defined here ->
template<class T>
bool DB<T>::put(Key<T> key, Value value) {

    return false;
}

template<class T>
bool DB<T>::del(Key<T> key) {
    return true;
}

template<class T>
Value DB<T>::get(Key<T> key) {
    return Value();
}

template<class T>
std::vector<Value> DB<T>::scan(Key<T> low, Key<T> high) {
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
