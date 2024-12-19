#ifndef UTILS_H
#define UTILS_H
#pragma once

#include "Option.h"
#include <string>
#include <vector>

void executeCommand(const std::string& command);
char getch();
void clearScreen();
int selectMenu(const std::vector<OptionMenu>& options, const std::string& title, const std::string& description);

#endif