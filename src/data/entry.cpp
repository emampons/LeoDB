#include <data/entry.h>

template <class T, class U>
Entry<T, U>::Entry(Key<T> _key, Value<U> _value) {
    key = _key;
    value = _value;
}

template <class T, class U>
Entry<T, U> ::Entry(std::string _key, std::string _value) {
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
    if (deleted == 1) {
        ret += "::::1";
    } else {
        ret += "::::0";
    }
    return ret;
}

template<class T, class U>
void Entry<T, U> :: tomb_it() {
    /*
     * Function tomb_it: Function to mark a entry as a tombstone (i.e. removed)
     */
    deleted = 1;
}

template<class T, class U>
bool Entry<T, U>::is_tomb(){
    /*
     * Function is_tomb: Helper function to see if a entry is a tombstone (i.e. removed)
     * Returns True/False if it is/is not a tombstone
     */
    return deleted;
}