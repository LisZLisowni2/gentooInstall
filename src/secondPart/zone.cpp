#include "InstallerSecond.hpppp"
#include "utils.hpp"
#include <string>
#include <iostream>
#include <stdexcept>
#include <map>
#include <vector>
#include <fstream>

void InstallerSecond::zoneConfig() {
    executeCommand("mv ./scripts/zone.sh /tmp/zone.sh");
    executeCommand("mv ./scripts/subZone.sh /tmp/subZone.sh");
    executeCommand("chmod +x /tmp/zone.sh");
    executeCommand("chmod +x /tmp/subZone.sh");
    executeCommand("/tmp/zone.sh");
    std::ifstream zoneFile("/tmp/zone.tmp");
    std::string line;
    std::vector<OptionMenu<std::string>> options = {};
    std::string lineValue;
    int index = 0;
    while (getline(zoneFile, line)) {
        if (!line.empty() && line[line.length() - 1] == '\n') {
            line.erase(line.length() - 1);
        }

        options.push_back(OptionMenu(line, index));
        index++;
    }

    clearScreen();
    int key = selectMenu<std::string>(options, "List of available zones", "Choose correct zone");
    std::cout << "\n";
    std::string zone = options[key].title;
    zoneFile.close();

    std::string rootZone = options[key].title;
    // executeCommand("rm /tmp/zone.tmp");
    executeCommand("/tmp/subZone.sh " + rootZone);
    std::ifstream subZoneFile("/tmp/subZone.tmp");
    options.clear();
    index = 0;
    while (getline(subZoneFile, line)) {
        if (!line.empty() && line[line.length() - 1] == '\n') {
            line.erase(line.length() - 1);
        }

        options.push_back(OptionMenu(line, index));
        index++;
    }

    clearScreen();
    key = selectMenu<std::string>(options, "List of available subzones", "Choose correct subzone");
    std::cout << "\n";
    // executeCommand("rm /tmp/zone.tmp");
    // executeCommand("rm /tmp/subZone.sh");
    // executeCommand("rm /tmp/zone.sh");
    executeCommand("ln -sf /usr/share/zoneinfo/" + zone + "/" + options[key].title + " /etc/localtime");
    zoneFile.close();
}