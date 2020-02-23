#include "key.h"

template<class T>
bool Key<T>::operator==(Key other) {
    return item == other.item;
}


template<class T>
bool Key<T>::operator<(Key other) {
    return item < other.item;
}

template<class T>
bool Key<T>::operator>(Key other) {
    return item > other.item;
}


