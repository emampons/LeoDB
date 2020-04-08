#include <fence_pointer/fence_pointer.h>

#include <utility>



void FencePointer::update_pointer(Entry<int, int> data, std::string file) {
    int key = data.getKey().getItem();
    pointer[key] = std::move(file);

}


int  FencePointer::get_size() {
    return total_entries;
}

Value<int> FencePointer::search(Key<int> key) {
    std::string file = pointer[key.getItem()];
    int hash = key.hashItem();
    LOAD_INTO_MEMORY(file);
    Entry<int, int> result = searches[hash];
    searches.clear();
    return result.getValue();
}


void FencePointer::LOAD_INTO_MEMORY(std::string file_name) {
    std::ifstream possible_file(file_name + "/in-memory.leodb");
    if (possible_file.is_open()) {
        DLOG_F(INFO, "Found in-memory.leodb");
        std::string output_text;
        while (getline (possible_file, output_text)) {
            Key<int> key(std::stoi(output_text.substr(output_text.find("::")+2, output_text.find(":::"))));
            Value<int> value(std::stoi(output_text.substr( output_text.find("::")+2, output_text.length())));
            int inserted = key.hashItem();

            searches[inserted] = Entry<int, int>(key, value);
        }
        DLOG_F(INFO, "Successfully loaded temporary database for fence pointer ");
    } else {
        DLOG_F(WARNING, "No file could be found for the fence pointer");
    }
    possible_file.close();

}
