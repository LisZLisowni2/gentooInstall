#include "InstallerFirst.h"
#include "utils.h"
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <map>

std::string InstallerFirst::partitionSelection() {
    executeCommand("(lsblk -rn | grep 'part' | awk '{ print $1 \"*\" $4 }') > /tmp/partitions.tmp");
    std::ifstream partitionsFile("/tmp/partitions.tmp");
    std::string line;
    std::vector<OptionMenu<std::string>> options = {};
    std::string lineValue;
    int index = 0;
    while (getline(partitionsFile, line)) {
        if (!line.empty() && line[line.length() - 1] == '\n') {
            line.erase(line.length() - 1);
        }

        lineValue = line.substr(0, line.find('*')); 
        options.push_back(OptionMenu(line, index, lineValue));
        index++;
    }

    clearScreen();
    int key = selectMenu<std::string>(options, "List of available partitions", "Choose correct partition. This operation is dangerous for your data!");
    std::cout << "\n";
 
    return options[key].value;
}

std::string InstallerFirst::formatSelection() {
    std::vector<OptionMenu<std::string>> options = {
        OptionMenu("ext4", 0),
        OptionMenu("fat32", 1),
        OptionMenu("ntfs", 2),
        OptionMenu("btrfs", 3),
        OptionMenu("zfs", 4),
        OptionMenu("exfat", 5),
    };
    
    clearScreen();
    int key = selectMenu<std::string>(options, "List of available formats", "Choose correct format. This operation is dangerous for your data!");
    std::cout << "\n";
 
    return options[key].title;
}

enum Formats {
    ext4 = 0,
    fat32 = 1,
    ntfs = 2,
    btrfs = 3,
    xfs = 4,
    exfat = 5,
};

void InstallerFirst::format() {
    std::vector<OptionMenu<std::string>> options = {
        OptionMenu("Format root partition (ext4 format)", 0),
        OptionMenu("Format boot partition", 1),
        OptionMenu("Format swap partition", 2),
        OptionMenu("Format other partition", 3),
        OptionMenu("Next", -1),
    };
    std::map<std::string, Formats> formatsMap = {
        {"ext4", ext4},
        {"fat32", fat32},
        {"ntfs", ntfs},
        {"btrfs", btrfs},
        {"xfs", xfs},
        {"exfat", exfat},
    };
    while (true) {
        clearScreen();
        int key = selectMenu(options, "Format partitions", "Format the partitions you want. In this point I will prompt you to input the partitions you want to format, on next screen instructions. Script will format root (ext4), swap and boot (fat32) partition. But if you want own special configuration (example you have home partition), you have to exit from this program using Ctrl+C right now. You can back to it, simply rerun the script and skip this part. The script remember the last procedding menu until the reboot.");
        std::cout << "\n";
        std::string partition;
        partition = partitionSelection();
        switch (key) {
            case -1:
                return;
                break;
            case 0:
                executeCommand("mkfs.ext4 " + partition);
                break;
            case 1:
                executeCommand("mkfs.fat -F 32 " + partition);
                break;
            case 2:
                executeCommand("mkswap " + partition);
                executeCommand("swapon " + partition);
                break;
            case 3:
                std::string format = formatSelection();
                Formats selectedFormat = formatsMap[format];
                switch (selectedFormat) {
                    case 0:
                        executeCommand("mkfs.ext4 " + partition);
                        break;
                    case 1:
                        executeCommand("mkfs.fat -F 32 " + partition);
                        break;
                    case 2:
                        executeCommand("mkfs.nsfat " + partition);
                        break;
                    case 3:
                        executeCommand("mkfs.btrfs " + partition);
                        break;
                    case 4:
                        executeCommand("mkfs.xfs " + partition);
                        break;
                    case 5:
                        executeCommand("mkfs.exfat " + partition);
                        break;
                }
                break;
        }
        std::cout << "\n\nPress any key to continue.\n";
        getch();
    }
}