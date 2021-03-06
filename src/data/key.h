#ifndef LEODB_KEY_H
#define LEODB_KEY_H

#include <string>
#include <iostream>
#include <loguru.cpp>

/*
 * Class representing the Key in the KV pair
 */
template<class T>
class Key {
public:
    Key() = default;
    ~Key() = default;
    explicit Key(T _item){item = _item; }
    bool operator == (Key other){ return item == other.item; }
    bool operator > (Key other){ return item > other.item; }
    bool operator < (Key other){ return item < other.item; }
    bool operator >= (Key other){ return item >= other.item; }
    bool operator <= (Key other){ return item <= other.item; }
    std::string getString();
    friend std::ostream& operator<<(std::ostream& os,  const Key& key){
        os << key.item << "\0";
        return os;
    }
    T getItem ();
    int hashItem();
private:
     T item;
};

#endif //LEODB_KEY_H
