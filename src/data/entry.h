#ifndef LEODB_ENTRY_H
#define LEODB_ENTRY_H

#include <data/value.cpp>
#include <data/key.cpp>
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
    void tomb_it();
    bool is_tomb();
private:
    Key<T> key;
    Value<U> value;
    int deleted;
};

#endif //LEODB_ENTRY_H
