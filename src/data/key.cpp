#include <data/key.h>

template<class T>
int Key<T>::hashItem() {
    /*
     * Function hashItem: Function to hash the key to turn it into an int
     * Return: Int hash version of the key
     */
    return item;
}

template<class T>
T Key<T>::getItem() {
    /*
     * Function getItem: Gets the Key item
     * Return: Key object
     */
    return item;
}

template<class T>
std::string Key<T>::getString() {
    /*
     * Function buildString: Get String representation of Key
     * Return: String representation of Key
     */
    try {
        return std::to_string(item);
    } catch (int e){
        LOG_F(ERROR, "Invalid Key item type");
        return "";
    }
}
