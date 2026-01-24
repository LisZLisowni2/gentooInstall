#include "InstallerFirst.hpp"
#include "utils.hpp"
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
        if (key == -1) return;
        std::string partition;
        partition = partitionSelection();
        switch (key) {
            case 0:
                executeCommand("mount " + partition + " /mnt/gentoo");
                break;
            case 1:
                if (isEFI) executeCommand("mount --mkdir " + partition + " /mnt/gentoo/boot/efi");
                else executeCommand("mount --mkdir " + partition + " /mnt/gentoo/boot/");
                break;
            case 2:
                std::string path;
                std::cout << "\nInput path where mount: ";
                std::getline(std::cin, path);
                executeCommand("mount --mkdir " + partition + " " + path);
                break;
            }
    }
}
