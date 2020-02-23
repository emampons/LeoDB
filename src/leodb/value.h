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

    explicit Value(T item){
        items.push_back(item);
    }
    explicit Value(std::vector<T> _items){
        items = _items;
    }
    ~Value()= default;
     bool operator == (Value other) const {
         return other.items == items;
     }

private:
    std::vector<T> items;

};


#endif //LEODB_VALUE_H
