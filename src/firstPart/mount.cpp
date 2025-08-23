#include "InstallerFirst.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <map>

void InstallerFirst::mount() {
    std::vector<OptionMenu<std::string>> options = {
        OptionMenu("Mount root partition", 0),
        OptionMenu("Mount boot partition", 1),
        OptionMenu("Mount other partition", 2),
        OptionMenu("Next", -1),
    };
    while (true) {
        clearScreen();
        int key = selectMenu(options, "Mouting partitions", "Mounted partitions means that you can directly access to data on. In this step mount every ppartition that you created. IMPORTANT! START FROM ROOT PARTITION! If you created home partition or any else doesn't mention before use Mount another option and define your own directory. If you have already mounted the partitions (like previous step) skip this. If you want manually mount partitions click Ctrl+C and come back after.");
        std::cout << "\n";
        std::string partition;
        executeCommand("fdisk -l");
        switch (key) {
            case -1:
                return;
                break;
            case 0:
                std::cout << "\n\nInput root partition (e.g. /dev/sda1): ";
                std::cin >> partition;
                executeCommand("mount " + partition + " /mnt/gentoo");
                break;
            case 1:
                std::cout << "\n\nInput boot partition (e.g. /dev/sda2): ";
                std::cin >> partition;
                if (isEFI) executeCommand("mount --mkdir " + partition + " /mnt/gentoo/boot/efi");
                else executeCommand("mount --mkdir " + partition + " /mnt/gentoo/boot/");
                break;
            case 2:
                std::string path;
                std::cout << "\n\nInput partition that you want mount: ";
                std::cin >> partition;
                std::cout << "\nInput path where mount: ";
                std::cin >> path;
                executeCommand("mount --mkdir " + partition + " " + path);
                break;
            }
        std::cout << "\n\nPress any key to continue.\n";
        getch();
    }
}
