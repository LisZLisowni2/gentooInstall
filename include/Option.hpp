#ifndef OPTION_HPP
#define OPTION_HPP
#pragma once

#include <string>

template <typename ValueType = std::string> class OptionMenu {
public:
    std::string title;
    int actionID;
    ValueType value;
    OptionMenu(std::string _title, int _actionID, ValueType _value = "IGNORE") {
        title = _title;
        actionID = _actionID;
        value = _value;
    };
};

#endif