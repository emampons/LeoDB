#include "entry.h"

template<class T, class U>
Entry<T, U>::Entry(Key<T> key, Value<U> value) {
    key = key;
    value = value;
}

template<class T, class U>
Value<U> Entry<T, U>::getValue() {
    return value;
}
