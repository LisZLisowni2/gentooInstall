#include "InstallerSecond.hpp"
#include "utils.hpp"
#include "makeMain.cpp"
#include "profile.cpp"
#include "kernel.cpp"
#include "networkConfig.cpp"
#include "bootloader.cpp"
#include "user.cpp"
#include "zone.cpp"
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <fstream>

InstallerSecond::InstallerSecond() {
    std::cout << "Gentoo Installer Initialized.\n"; 
    executeCommand("uname -m > /tmp/uname.tmp");
    executeCommand("echo $([ -d /sys/firmware/efi ] && echo UEFI || echo BIOS) > /tmp/uefi.tmp");
    executeCommand("echo $(! rc-service --version && echo SYSTEMD || echo OPENRC) > /tmp/init.tmp");
    std::ifstream unameFile("/tmp/uname.tmp");
    std::ifstream uefiFile("/tmp/uefi.tmp");
    std::ifstream initFile("/tmp/init.tmp");
    std::string line;
    while (getline(unameFile, line)) {
        if (!line.empty() && line[line.length() - 1] == '\n') {
            line.erase(line.length() - 1);
        }

        if (line == "x86_64") isARM = false;
        else if (line == "aarch64" || line == "arm64") isARM = true;
        else {
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
    while (getline(initFile, line)) {
        if (!line.empty() && line[line.length() - 1] == '\n') {
            line.erase(line.length() - 1);
        }

        if (line == "OPENRC") isOpenRC = true;
        else isOpenRC = false;
    }
    initFile.close();
}

InstallerSecond::~InstallerSecond() {
    std::cout << "Gentoo Installer Completed.\n"; 
}

void InstallerSecond::install() {
    executeCommand("emerge-webrsync");
    makeMain();
    profile();
    executeCommand("emerge --autounmask-continue --deep --update --newuse --verbose @world");
    zoneConfig();
    kernelConfig();
    kernelInstall();
    networkConfig();
    bootloader();
    userCreation();
    std::cout << "\n\nYou have finished installation of minimal Gentoo.\n";
    getch();
    executeCommand("exit");
}