#include <data/entry.h>

template <class T, class U>
Entry<T, U>::Entry(Key<T> _key, Value<U> _value) {
    key = _key;
    value = _value;
}

template<class T, class U>
Value<U> Entry<T, U>::getValue() {
    return value;
}

template<class T, class U>
Key<T> Entry<T, U>::getKey() {
    return key;
}