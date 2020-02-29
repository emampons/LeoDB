#ifndef LEODB_ENTRY_H
#define LEODB_ENTRY_H

#include <data/value.h>
#include <data/key.h>

/*
 * Class representing the KV pair as one object, entry
 */
template <class T, class U>
class Entry {
public:
    Entry() = default;
    Entry(Key<T> _key, Value<U> _value);
    Key<T> getKey();
    Value<U> getValue();
private:
    Key<T> key;
    Value<U> value;
};

#endif //LEODB_ENTRY_H
