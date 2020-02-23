#ifndef LEODB_ENTRY_H
#define LEODB_ENTRY_H


#include "key.h"
#include "value.h"

template <class T, class U>
class Entry {
public:
    Entry(Key<T> key, Value<U> value);
    Value<U> getValue();
private:
    Key<T> key;
    Value<U> value;
};


#endif //LEODB_ENTRY_H
