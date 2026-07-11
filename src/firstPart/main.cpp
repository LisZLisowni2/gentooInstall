#include "InstallerFirst.hpp"
#include "utils.hpp"
#include <iostream>

int main() {
    clearScreen();
    InstallerFirst installer;
    installer.install();
    std::cout << "\nInstallation completed. Reboot system (`sudo reboot`) or chroot to system using `sudo chroot /mnt/gentoo bash`";
    return 0;
}