#ifndef OPTION_H
#define OPTION_H
#pragma once

#include <string>

template<typename T> class OptionMenu {
public:
    std::string title;
    int actionID;
    T value;
    OptionMenu(std::string _title, int _actionID, T _value = NULL);
};

#endif