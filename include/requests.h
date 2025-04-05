#ifndef REQUESTS_H
#define REQUESTS_H
#pragma once

#include "Option.h"
#include "utils.h"
#include <vector>
#include <cstdlib>
#include <string>
#include <regex>

std::string latestVersion(const bool& isARM);
bool isNewer(const std::string& folderName1, const std::string& folderName2);
std::tm parseDate(const std::string& folderName);
std::vector<std::string> getFoldersFromServer(const std::string& url);

#endif