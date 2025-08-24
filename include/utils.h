#ifndef UTILS_H
#define UTILS_H
#pragma once

#include "Option.h"
#include "utils.h"
#include <vector>
#include <cstdlib>
#include <string>
#include <regex>

int executeCommand(const std::string& command);
void installPackages(const std::string& packages); // For masked packages, dependency conflicts
char getch();
void clearScreen();
template <typename ValueType = std::string>
int selectMenu(const std::vector<OptionMenu<ValueType>>& options, const std::string& title, const std::string& description); 
std::string latestVersion();
bool isNewer(const std::string& folderName1, const std::string& folderName2);
std::tm parseDate(const std::string& folderName);
std::vector<std::string> getFoldersFromServer(const std::string& url);
void helpBox();

#endif