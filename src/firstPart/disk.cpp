#include "InstallerFirst.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <fstream>

void InstallerFirst::partitions() {
    executeCommand("(lsblk -rd | awk '!/NAME/ { print $1 }') > /tmp/devices.tmp");
    std::ifstream deviceFile("/tmp/devices.tmp");
    std::string line;
    std::vector<OptionMenu> options = {};
    int index = 0;
    while (getline(deviceFile, line)) {
        if (!line.empty() && line[line.length() - 1] == '\n') {
            line.erase(line.length() - 1);
        }

        options.push_back(OptionMenu(line, index));
        index++;
    }

    options.push_back(OptionMenu("Example layouts (Not working for now)", -2));
    bool optionContinueCreated = false;
    while (true)
    {
        clearScreen();
        std::cout << "UEFI mode: " << isEFI << "\n";
        int key = selectMenu(options, "Partitions", "You have to configure a partitions to install the system. It depends on how you would like to configure the partitions, but if you doesn't have any idea how to configure, there are example layouts in file. Choose option 'Example layouts' to open it. WARNING! Be careful, you can accidentally erase data on other partitions. Make backup before any changes!");
        std::cout << "\n";
        
        if (key == -1) {
            return;
        }

        if (key == -2) {
            // executeCommand("cat  | less"); TODO: Download from another github repo example layouts
            continue;
        }

        std::string device = options[key].title;
        executeCommand("cfdisk /dev/" + device);
        
        if (!optionContinueCreated) { 
            options.push_back(OptionMenu("Continue", -1));
            optionContinueCreated = true;
        }
    }
}