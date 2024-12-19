#include "Option.h"
#include <string>
#include <iostream>
#include <stdexcept>

OptionMenu::OptionMenu(std::string _title, int _actionID) {
    title = _title;
    actionID = _actionID;
}