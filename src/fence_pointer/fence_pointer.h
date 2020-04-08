#ifndef LEODB_FENCE_POINTER_H
#define LEODB_FENCE_POINTER_H

#include <unordered_map>
#include <data/entry.h>
#include <string>
#include <data/value.h>
#include <fstream>

//template <class T, class U>
class FencePointer {
public:
    FencePointer() {
        total_entries = 0;
    };

    ~FencePointer() = default;

    void update_pointer(Entry <int, int> data, std::string file);
    Value<int> search(Key<int> key);
    int get_size();

private:
    std::unordered_map<int, std::string> pointer;
    int MAX_SIZE = 1024;
    int total_entries;
    void LOAD_INTO_MEMORY(std::string file_name);
    std::unordered_map<int, Entry<int, int> > searches;

};


#endif //LEODB_FENCE_POINTER_H
