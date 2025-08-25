#include "InstallerFirst.h"
#include "utils.h"
#include <iostream>

int main() {
    clearScreen();
    InstallerFirst installer;
    installer.install();
    std::cout << "\nDo you want reboot a system now? [Y/n]";
    char rebootOption = getch();
    switch (rebootOption) {
        case 'n':
        case 'N':
            std::cout << "\nIf you want later reboot, input exit";
            getch();
            executeCommand("chroot /mnt/gentoo bash");
    }
    executeCommand("reboot");
    return 0;
}