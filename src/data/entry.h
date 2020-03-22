#ifndef LEODB_ENTRY_H
#define LEODB_ENTRY_H

#include <data/value.h>
#include <data/key.h>
#include <type_traits>
#include <loguru.cpp>

/*
 * Class representing the KV pair as one object, entry
 */
template <class T, class U>
class Entry {
public:
    Entry() = default;
    Entry(Key<T> _key, Value<U> _value);
    Entry(std::string _key, std::string _value);
    Key<T> getKey();
    Value<U> getValue();
    std::string buildString();
private:
    Key<T> key;
    Value<U> value;
};

#endif //LEODB_ENTRY_H
