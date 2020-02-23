#ifndef LEODB_KEY_H
#define LEODB_KEY_H

#include "string"
#include "iostream"


template  <typename T>
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


};



#endif //LEODB_KEY_H
