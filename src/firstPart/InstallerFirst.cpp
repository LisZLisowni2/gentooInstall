#include "InstallerFirst.h"
#include "startMenu.cpp"
#include "networkConfig.cpp"
#include "format.cpp"
#include "disk.cpp"
#include "mount.cpp"
#include "chroot.cpp"
#include "tarball.cpp"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstdlib>

InstallerFirst::InstallerFirst() {
    std::cout << "Gentoo Installer Initialized.\n";
    executeCommand("uname -m > /tmp/uname.tmp");
    executeCommand("echo $([ -d /sys/firmware/efi ] && echo UEFI || echo BIOS) > /tmp/uefi.tmp");
    std::ifstream unameFile("/tmp/uname.tmp");
    std::ifstream uefiFile("/tmp/uefi.tmp");
    std::string line;
    while (getline(unameFile, line)) {
        if (!line.empty() && line[line.length() - 1] == '\n') {
            line.erase(line.length() - 1);
        }

        if (line == "x86_64") {
            isARM = false;
        } else if (line == "aarch64" || line == "arm64") {
            isARM = true;
        } else {
            std::cout << "Unknown architecture. Select default option";
            isARM = false;
        }
    }
    unameFile.close();
    while (getline(uefiFile, line)) {
        if (!line.empty() && line[line.length() - 1] == '\n') {
            line.erase(line.length() - 1);
        }

        if (line == "UEFI") isEFI = true;
        else isEFI = false;
    }
    uefiFile.close();
}

InstallerFirst::~InstallerFirst() {
    std::cout << "Gentoo Installer Completed.\n"; 
}

void InstallerFirst::install() {
    startMenu();
    networkConfig();
    partitions();
    format();
    mount();
    tarball();
    chroot();
}
