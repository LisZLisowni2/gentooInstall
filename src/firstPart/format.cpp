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

void InstallerFirst::format() {
    std::vector<OptionMenu<std::string>> options = {
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
        int key = selectMenu(options, "Format partitions", "Format the partitions you want. In this point I will prompt you to input the partitions you want to format, on next screen instructions. Script will format root (ext4), swap and boot (fat32) partition. But if you want own special configuration (example you have home partition), you have to exit from this program using Ctrl+C right now. You can back to it, simply rerun the script and skip this part. The script remember the last procedding menu until the reboot.");
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
                executeCommand("swapon " + partition);
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