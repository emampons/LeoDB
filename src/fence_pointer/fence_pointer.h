#ifndef LEODB_FENCE_POINTER_H
#define LEODB_FENCE_POINTER_H

#include <unordered_map>
#include "entry.h"
#include <string>
class FencePointer {
public:
    FencePointer(){total_entries:0;};
    ~FencePointer() = default;
    void update_pointer(Entry<T, U> data, std::string file);
    int get_size();
    void flush_data();
    void load_data();

private:
    std::unorded_map pointer<std::string file, Entry<T, U> data);
    int MAX_SIZE = 1024;
    int total_entries;
    const std::string meta_data = "TODO";

};


#endif //LEODB_FENCE_POINTER_H
