#include "utils.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <stdexcept>

void executeCommand(const std::string& command) {
    int result = std::system(command.c_str());
    if (result != 0) {
        throw std::runtime_error("Command failed: " + command);
    }
}