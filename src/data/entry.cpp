#include <data/entry.h>

template <class T, class U>
Entry<T, U>::Entry(Key<T> _key, Value<U> _value) {
    key = _key;
    value = _value;
}

template <class T, class U>
Entry<T, U>::Entry(std::string _key, std::string _value) {
    /*
     * Function Constructor: Build a entry from two strings
     */
    if (std::is_same_v<T, int> && std::is_same_v<U, int>){
        key = Key<T>(std::stoi(_key));
        value = Value<U>(std::stoi(_value));
    } else {
        LOG_F(ERROR, "Invalid type passed in as string for Entry()");
        exit(1);
    }
}

template<class T, class U>
Value<U> Entry<T, U>::getValue() {
    return value;
}

template<class T, class U>
Key<T> Entry<T, U>::getKey() {
    return key;
}

template<class T, class U>
std::string Entry<T, U>::buildString() {
    /*
     * Function buildString: Gets a string representation of the entry to store in our DB
     * Return: String representation ready to be stored
     */
    std::string ret = std::to_string(key.hashItem()) + "::" + key.getString() + ":::" + value.getString();
    return ret;
}