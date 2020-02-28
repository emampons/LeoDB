#ifndef LEODB_VALUE_H
#define LEODB_VALUE_H

#include <vector>
/*
 * Class representing the value in the KV pair
 */
template <typename T>
class Value {
public:
    Value()= default;
    explicit Value(T _item){ item = _item;}
    ~Value()= default;
     bool operator == (Value other) const {
         return other.item == item;
     }
    bool operator > (Value other){
        return item > other.item;
    }

    bool operator < (Value other){
        return item < other.item;
    }
    bool operator >= (Value other){
        return item >= other.item;
    }
    bool operator <= (Value other){
        return item <= other.item;
    }
     T getItem(){ return item; }
private:
    T item;

};


#endif //LEODB_VALUE_H
