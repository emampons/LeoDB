#include <fence_pointer/fence_pointer.h>

#include <utility>


template<class T, class U>
void FencePointer<T, U>::update_pointer(Entry<T, U> data, std::string file) {
    int key = data.getKey().getItem();
    pointer[key] = std::move(file);

}

template<class T, class U>
int  FencePointer<T, U>::get_size() {
    return total_entries;
}

template<class T, class U>
Entry<T, U> FencePointer<T, U>::search(Key<T> key) {
    std::string file = pointer[key.getItem()];
    int hash = key.hashItem();
    LOAD_INTO_MEMORY(file);
    if (searches.size() > 0) {
        Entry<int, int> result = searches[hash];
        searches.clear();
        return result;
    }
    return Entry<T, U>(Key(key.getItem()+1), Value(-1));
}


template<class T, class U>
void FencePointer<T, U>::LOAD_INTO_MEMORY(std::string file_name) {
    std::ifstream possible_file(file_name);
    if (possible_file.is_open()) {
        std::string output_text;
        while (getline (possible_file, output_text)) {
            Key<int> key(std::stoi(output_text.substr(output_text.find("::")+2, output_text.find(":::"))));
            Value<int> value(std::stoi(output_text.substr( output_text.find("::")+2, output_text.length())));
            int inserted = key.hashItem();

            searches[inserted] = Entry<T, U>(key, value);
        }
        DLOG_F(INFO, "Successfully loaded temporary database for fence pointer ");
    } else {
        DLOG_F(WARNING, "No file could be found for the fence pointer");
        searches.clear();
    }
    possible_file.close();

}
