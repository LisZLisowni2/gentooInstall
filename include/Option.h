#ifndef OPTION_H
#define OPTION_H
#pragma once

#include <string>

class OptionMenu {
public:
    std::string title;
    int actionID;
    OptionMenu(std::string _title, int _actionID);
};

#endif