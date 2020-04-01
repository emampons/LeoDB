#include <data/value.h>

template <typename U>
U Value<U>::getItem() {
    /*
     * Function getItem: Returns the item inside the value object
     * Returns: Item of type U
     */
    return item;
}

template<class U>
std::string Value<U>::getString() {
    /*
     * Function buildString: Get String representation of Value
     * Return: String representation of Value
     */
    try {
        return std::to_string(item);
    } catch (int e){
        LOG_F(ERROR, "Invalid Value item type");
        return "";
    }
}
