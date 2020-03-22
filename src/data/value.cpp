#include <data/value.h>

template <typename U>
U Value<U>::getItem() {
    /*
     * Function getItem: Returns the item inside the value object
     * Returns: Item of type U
     */
    return item;
}
