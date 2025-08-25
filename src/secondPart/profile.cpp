#include "InstallerSecond.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <map>
#include <vector>
#include <fstream>

int InstallerSecond::profileSelection() { 
    executeCommand("(eselect profile list | grep \"default\" | sed 's/ //g; s/\\[//g; s/\\]/?/g') > /tmp/profiles.tmp");
    std::ifstream profilesFile("/tmp/profiles.tmp");
    std::string line;
    int value;
    std::vector<OptionMenu<int>> options = {};
    int index = 0;
    while (getline(profilesFile, line)) {
        if (!line.empty() && line[line.length() - 1] == '\n') {
            line.erase(line.length() - 1);
        }

        std::string numberString = line.substr(0, line.find('?'));
        std::cout << numberString << "\n";
        value = std::stoi(numberString);
        options.push_back(OptionMenu<int>(line, index, value));
        index++;
    }

    clearScreen();
    int key = selectMenu(options, "List of available profiles", "Remember which tarball you picked");
    std::cout << "\n";

    return options[key].value;
}

void InstallerSecond::profile() {
    std::vector<OptionMenu<std::string>> options = {
        OptionMenu("Set profile", 0),
        OptionMenu("Next", -1),
    };
    while (true) {
        clearScreen();
        int key = selectMenu(options, "Profile", "In Gentoo profiles defines the basic packages to be installed. The default profile depends on extracted tarball. If you want change profile select the second option");
        std::cout << "\n";
        switch (key) {
            case -1:
                return;
                break;
            case 0:
                int id = profileSelection();
                executeCommand("eselect profile set " + std::to_string(id)); 
                break;
        }
        std::cout << "\n\nPress any key to continue.\n";
        getch();
    }
}
