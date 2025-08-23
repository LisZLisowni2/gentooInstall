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
    }

    bool optionContinueCreated = false;
    while (true)
    {
        clearScreen();
        std::cout << "UEFI mode: " << isEFI << "\n";
        int key = selectMenu(options, "Partitions", "You have to configure a partitions to install the system. At this point depends on you how would you like to configure the partitions, but if you doesn't have any idea how configure, there are the example layout. Example layout:	/dev/sda1 1GB EFI Partition | /dev/sda2 RAMsize * 2 SWAP Partition | /dev/sda3 'remainder of the disk' Root partition.		If you want home partition, division the root for two parts: root partition and home partition. WARNING! Be careful, you can accidentally erase data on other partitions. Make backup before any changes!");
        std::cout << "\n";
    
        std::string device = options[key].title;
        executeCommand("cfdisk " + device);
        
        if (!optionContinueCreated) { 
            options.push_back(OptionMenu("Continue", -1));
            optionContinueCreated = true;
        }
    }
}