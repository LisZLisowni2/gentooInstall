#include "Option.h"
#include <string>
#include <iostream>
#include <stdexcept>

template<typename T>
OptionMenu<T>::OptionMenu(std::string _title, int _actionID, T _value = NULL) {
    title = _title;
    actionID = _actionID;
    value = _value;
}