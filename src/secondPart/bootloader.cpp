#include "InstallerSecond.hpp"
#include "utils.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <map>
#include <vector>

std::string InstallerSecond::diskSelection() {
    executeCommand("(lsblk -rd | awk '!/NAME/ { print $1 }') > /tmp/disks.tmp");
    std::ifstream disksFile("/tmp/disks.tmp");
    std::string line;
    std::vector<OptionMenu<std::string>> options = {};
    std::string lineValue;
    int index = 0;
    while (getline(disksFile, line)) {
        if (!line.empty() && line[line.length() - 1] == '\n') {
            line.erase(line.length() - 1);
        }

        options.push_back(OptionMenu(line, index, "/dev/" + line));
        index++;
    }

    clearScreen();
    int key = selectMenu<std::string>(options, "List of available disks", "Choose disk where you installed an system.");
    std::cout << "\n";
 
    return options[key].value;
}

void InstallerSecond::bootloader() {
    if (isEFI) executeCommand("echo \"sys-boot/grub efi\" > /etc/portage/portage.use/grub");
    installPackages("sys-boot/grub");
    if (isEFI) executeCommand("grub-install --target=x86_64-efi --efi-directory=/boot/efi --bootloader-id=GENTOO");
    else  {
        std::string disk = diskSelection();
        executeCommand("grub-install --target=i386-pc " + disk); 
    }
    executeCommand("grub-mkconfig -o /boot/grub/grub.cfg");
    executeCommand("passwd");
}