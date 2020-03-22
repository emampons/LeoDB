#ifndef LEODB_VALUE_H
#define LEODB_VALUE_H

#include <vector>
#include <loguru.cpp>

/*
 * Class representing the value in the KV pair
 */
template <typename U>
class Value {
public:
    Value() = default;
    ~Value() = default;
    explicit Value(U _item){ item = _item; }
    bool operator == (Value other) const { return other.getItem() == item; }
    bool operator > (Value other){ return item > other.getItem(); }
    bool operator < (Value other){ return item < other.getItem(); }
    bool operator >= (Value other){ return item >= other.getItem(); }
    bool operator <= (Value other) { return item <= other.getItem(); };
    std::string getString();
    U getItem();
private:
    U item;

};


#endif //LEODB_VALUE_H
