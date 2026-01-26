#include "InstallerSecond.hpp"
#include "utils.hpp"
#include <string>
#include <iostream>
#include <stdexcept>
#include <map>
#include <vector>
#include <fstream>

void InstallerSecond::localeSelection() {
    executeCommand("(cat /etc/locale.gen | tail -n+3 | awk '{ print $2 }') > /tmp/localeValues.tmp");
    executeCommand("(cat /etc/locale.gen | tail -n+3 | awk \'{ s = \"\"; for (i = 4; i <= NF; i++) s = s \" \" $i; print s }\') > /tmp/localeNames.tmp");
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

        values.push_back(line);
    }

    for (int i = 0; i < names.size(); i++) {
        options.push_back(OptionMenu(values[i] + " " + names[i], index, values[i]));
        index++;
    }

    clearScreen();
    int key = selectMenu(options, "List of available locales", "Choose correct locale");
    std::cout << "\n";
    if (options[key].value.find('@') == std::string::npos) {
        executeCommand("echo \"" + options[key].value + ".UTF-8 UTF-8\" >> /etc/locale.gen");
    } else {
        std::string newString = options[key].value.substr(0, options[key].value.find("@") - 1) + ".UTF-8" + options[key].value.substr(options[key].value.find("@") - 1) + " UTF-8";
        executeCommand("echo \"" + newString + "\" > /etc/locale.gen");
    }
}

void InstallerSecond::localeConfig() {
    std::vector<OptionMenu<std::string>> options = {
        OptionMenu("Add locale", 1),
        OptionMenu("Next", -1),
    };

    while (true) {
        localeSelection();
        clearScreen();
        int key = selectMenu(options, "Do you want add more locales?", "Choose correct option");
        std::cout << "\n";
        if (key == -1) {
            break;
        }
    }

    executeCommand("locale-gen");
    executeCommand("eselect locale list | tail -n+1 | awk '{ print $2 }' > /tmp/locales.tmp");
    std::ifstream localeFile("/tmp/locales.tmp");
    std::string line;
    int value;
    options.clear();
    int index = 0;
    while (getline(localeFile, line)) {
        if (!line.empty() && line[line.length() - 1] == '\n') {
            line.erase(line.length() - 1);
        }

        options.push_back(OptionMenu<std::string>(line, index));
        index++;
    }

    clearScreen();
    int key = selectMenu(options, "List of generated locales", "Choose correct locale");
    std::cout << "\n";
    executeCommand("eselect locale set " + options[key].title);
}