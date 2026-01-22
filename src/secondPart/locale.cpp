#include "InstallerSecond.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <map>
#include <vector>
#include <fstream>

void InstallerSecond::zoneConfig() {
    executeCommand("cat<<EOF>/tmp/locale.sh\n#!/bin/sh\n\nZONES=$(find /usr/share/zoneinfo -type d)\n\nfor ZONE in ${ZONES[@]}; do\n        RES=${ZONE##*/};\n        if [[ \"$RES\" != \"zoneinfo\" ]]; then\n                echo $RES\n        fi\ndone\n\nEOF");
    executeCommand("chmod +x /tmp/locale.sh");
    executeCommand("/tmp/locale.sh > /tmp/locale.tmp");
    std::ifstream localeFile("/tmp/locale.tmp");
    std::string line;
    std::vector<OptionMenu<std::string>> options = {};
    std::string lineValue;
    int index = 0;
    while (getline(localeFile, line)) {
        if (!line.empty() && line[line.length() - 1] == '\n') {
            line.erase(line.length() - 1);
        }

        options.push_back(OptionMenu(line, index));
        index++;
    }

    clearScreen();
    int key = selectMenu<std::string>(options, "List of available locales", "Choose correct locale");
    std::cout << "\n";
 
    executeCommand("");
}