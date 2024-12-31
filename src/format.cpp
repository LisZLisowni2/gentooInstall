#include "InstallerFirst.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <map>

enum Formats {
    EXT4 = 0,
    FAT32 = 1,
    EXT3 = 2,
    NTFS = 3
};

void InstallerFirst::partitions() {
    std::vector<OptionMenu> options = {
        OptionMenu("Format root partition", 0),
        OptionMenu("Format boot partition", 1),
        OptionMenu("Format swap partition", 2),
        OptionMenu("Format other partition", 3),
        OptionMenu("Next", -1),
    };
    std::map<std::string, Formats> formatsMap = {
        {"EXT4", EXT4},
        {"FAT32", FAT32},
        {"EXT3", EXT3},
        {"NTFS", NTFS},
    };
    while (true) {
        clearScreen();
        int key = selectMenu(options, "Partitions", "You have to configure a partitions to install the system. At this point depends on you how would you like to configure the partitions, but if you doesn't have any idea how configure, there are the example layout. Example layout:	/dev/sda1 1GB EFI Partition | /dev/sda2 RAMsize * 2 SWAP Partition | /dev/sda3 'remainder of the disk' Root partition.		If you want home partition, division the root for two parts: root partition and home partition. WARNING! Be careful, you can accidentally erase data on other partitions. Before procedding make backup!");
        std::cout << "\n";
        std::string partition;
        executeCommand("fdisk -l");
        switch (key) {
            case -1:
                return;
                break;
            case 0:
                std::cout << "\n\nInput partition that you want to format at ext4 type (root partition, e.g. /dev/sda1): ";
                std::cin >> partition;
                executeCommand("mkfs.ext4 " + partition);
                break;
            case 1:
                std::cout << "\n\nInput partition that you want to format at FAT32 type (boot partition, e.g. /dev/sda2): ";
                std::cin >> partition;
                executeCommand("mkfs.fat -F 32 " + partition);
                break;
            case 2:
                std::cout << "\n\nInput partition that you want to format to swap partition (e.g. /dev/sda3): ";
                std::cin >> partition;
                executeCommand("mkswap " + partition);
                break;
            case 3:
                std::string format;
                std::cout << "\n\nInput partition that you want to format at your type (e.g. home partition /dev/sda4): ";
                std::cin >> partition;
                std::cout << "\n\nInput format you want to format that partition (e.g. ext4, fat32, ext3, ntfs <-- These are already supported): ";
                std::cin >> format;
                std::transform(format.begin(), format.end(), format.begin(), ::toupper);
                Formats selectedFormat = formatsMap[format];
                switch (selectedFormat) {
                    case 0:
                        executeCommand("mkfs.ext4 " + partition);
                        break;
                    case 1:
                        executeCommand("mkfs.fat -F 32 " + partition);
                        break;
                    case 2:
                        executeCommand("mkfs.ext3 " + partition);
                        break;
                    case 3:
                        executeCommand("mkfs.ntfs " + partition);
                        break;
                }
                break;
        }
        std::cout << "\n\nPress any key to continue.\n";
        getch();
    }
}