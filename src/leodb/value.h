#ifndef LEODB_VALUE_H
#define LEODB_VALUE_H

#include <vector>
/*
 * Class representing the value in the KV pair
 */

class Value {
public:
    Value();
    Value(std::vector<int> _items);
    ~Value();
     bool operator == (Value other) const {
         return other.items == items;
     }

private:
    std::vector<int> items;

};


#endif //LEODB_VALUE_H
