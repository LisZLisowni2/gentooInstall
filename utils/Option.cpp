#include "Option.h"
#include <string>
#include <iostream>
#include <stdexcept>

template<typename ValueType>
OptionMenu<ValueType>::OptionMenu(std::string _title, int _actionID, ValueType _value = "IGNORE") {
    title = _title;
    actionID = _actionID;
    value = _value;
}