#ifndef LEODB_KEY_H
#define LEODB_KEY_H

#include "string"
#include "iostream"

template<class T>
class Key{
public:
    explicit Key(T _item){item = _item;}
    ~Key()= default;
    bool operator == (Key other){
        return item == other.item;
    }

    bool operator > (Key other){
        return item > other.item;
    }

    bool operator < (Key other){
        return item < other.item;
    }
    friend std::ostream& operator<<(std::ostream& os,  const Key& key){
        os << key.item << "\0";
        return os;
    }
    int hashItem() const{return item;};
private:
     T item;
};




#endif //LEODB_KEY_H
