#include "InstallerSecond.h"
#include <string>
#include "utils.h"
#include <iostream>
#include <stdexcept>

void InstallerSecond::utils() {
    installPackages("sudo genfstab app-portage/gentoolkit");
    executeCommand("genfstab -U / >> /etc/fstab");
    executeCommand("echo \"%wheel ALL=(ALL:ALL) ALL\" >> /etc/sudoers");
    clearScreen();
}