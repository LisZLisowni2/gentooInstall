#ifndef UTILS_HPP
#define UTILS_HPP
#pragma once

#include "Option.hpp"
#include <string>

char getch();
void clearScreen();
int executeCommand(const std::string& command);
void installPackages(const std::string& packages); // For masked packages, dependency conflicts

#endif