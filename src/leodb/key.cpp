#include "key.h"

template<class T>
bool Key<T>::operator==(Key<T> other) {
    return item == other.item;
}


template<class T>
bool Key<T>::operator<(Key<T> other) {
    return item < other.item;
}

template<class T>
bool Key<T>::operator>(Key<T> other) {
    return item > other.item;
}

template<class T>
int Key<T>::hashItem() {
    return item;
}


