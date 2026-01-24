#include "InstallerSecond.hpp"
#include "utils.hpp"
#include <string>
#include <iostream>
#include <stdexcept>
#include <map>
#include <vector>
#include <fstream>

void localeSelection() {
    executeCommand("(grep \"UTF-8\" /etc/locale.gen | awk '{ print $2 }') > /tmp/localeValues.tmp");
    executeCommand("(grep \"UTF-8\" /etc/locale.gen | awk '{ s = \"\"; for (i = 5; i <= NF; i++) s = s \" \" $i; print s }') > /tmp/localeNames.tmp");
    std::ifstream localeNameFile("/tmp/localeNames.tmp");
    std::ifstream localeValueFile("/tmp/localeValues.tmp");
    std::string line;
    int value;
    std::vector<OptionMenu<std::string>> options = {};
    std::vector<std::string> names = {};
    std::vector<std::string> values = {};
    int index = 0;
    while (getline(localeNameFile, line)) {
        if (!line.empty() && line[line.length() - 1] == '\n') {
            line.erase(line.length() - 1);
        }

        names.push_back(line);
    }

    while (getline(localeValueFile, line)) {
        if (!line.empty() && line[line.length() - 1] == '\n') {
            line.erase(line.length() - 1);
        }

        names.push_back(line);
    }

    for (int i = 0; i < names.size(); i++) {
        options.push_back(OptionMenu(names[i], index, values[i]));
        index++;
    }

    clearScreen();
    int key = selectMenu(options, "List of available locales", "Choose correct locale");
    std::cout << "\n";
    executeCommand("echo " + options[key].value + " > /etc/locale.gen");
}

void InstallerSecond::localeConfig() {
    
}