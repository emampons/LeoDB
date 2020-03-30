#include "fence_pointer.h"


void FencePointer::update_pointer(Entry<T, U> data, std::string file) {
    int key = data.getKey();
    pointer[file] = data;
}

int FencePointer::get_size() {
    return total_entries;
}

void FencePointer::flush_data() {}

void FencePointer::load_data() {}
