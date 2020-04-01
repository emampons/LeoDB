#include <data/key.h>

template<class T>
int Key<T>::hashItem() {
    /*
     *
     */
    return item;
}

template<class T>
T Key<T>::getItem() {
    /*
     *
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
