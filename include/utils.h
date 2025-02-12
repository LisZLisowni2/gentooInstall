#ifndef UTILS_H
#define UTILS_H
#pragma once

#include "Option.h"
#include <string>
#include <vector>

int executeCommand(const std::string& command);
void installPackages(const std::string& packages); // For masked packages, dependency conflicts
char getch();
void clearScreen();
int selectMenu(const std::vector<OptionMenu>& options, const std::string& title, const std::string& description); 

#endif