#ifndef LEODB_KEY_H
#define LEODB_KEY_H

#include "string"
#include "iostream"

template<class T>
class Key{
public:
    explicit Key(T _item){item = _item;}
    ~Key()= default;
    bool operator == (Key other);
    bool operator > (Key other);
    bool operator < (Key other);
    friend std::ostream& operator<<(std::ostream& os,  const Key& key){
        os << key.item << "\0";
        return os;
    }
private:
     T item;
    int hashItem();
};


#endif //LEODB_KEY_H
