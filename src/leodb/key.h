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
    int hashItem() const;
private:
     T item;
};

// Hash function used for key.item
template<class T>
class hashFunction{
public:
    // Use sum of lengths of first and last names
    // as hash function.
    size_t operator()(const Key<T>& key) const
    {
        return key.hashItem();
    }
};



#endif //LEODB_KEY_H
