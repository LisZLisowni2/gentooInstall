#include "InstallerFirst.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>

void InstallerFirst::partitions() {
    std::vector<OptionMenu> options = {
        OptionMenu("Configure disk", 0),
        OptionMenu("Next", -1),
    };
    while (true) {
        clearScreen();
        int key = selectMenu(options, "Partitions", "You have to configure a partitions to install the system. At this point depends on you how would you like to configure the partitions, but if you doesn't have any idea how configure, there are the example layout. Example layout:	/dev/sda1 1GB EFI Partition | /dev/sda2 RAMsize * 2 SWAP Partition | /dev/sda3 'remainder of the disk' Root partition.		If you want home partition, division the root for two parts: root partition and home partition. WARNING! Be careful, you can accidentally erase data on other partitions. Before procedding make backup!");
        std::cout << "\n";
        switch (key) {
            case -1:
                return;
                break;
            case 0:
                std::string dev;
                executeCommand("fdisk -l");
                std::cout << "\n\nInput disk which you want to repartition (e.g. /dev/sda, /dev/sdb, /dev/vda, etc.): ";
                std::cin >> dev;
                executeCommand("cfdisk " + dev);
                break;
        }
        std::cout << "\n\nPress any key to continue.\n";
        getch();
    }
}