#include "InstallerSecond.h"
#include "utils.h"
#include "makeMain.cpp"
#include "profile.cpp"
#include "kernel.cpp"
#include "networkConfig.cpp"
#include "bootloader.cpp"
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <fstream>

InstallerSecond::InstallerSecond() {
    std::cout << "Gentoo Installer Initialized.\n"; 
    executeCommand("uname -m > /tmp/uname.tmp");
    executeCommand("echo $([ -d /sys/firmware/efi ] && echo UEFI || echo BIOS) > /tmp/uefi.tmp");
    executeCommand("echo $(! rc-service && echo SYSTEMD || echo OPENRC) > /tmp/init.tmp");
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
    kernelConfig();
    kernelInstall();
    installPackages("genfstab");
    executeCommand("genfstab -U / >> /etc/fstab");
    clearScreen();
    networkConfig();
    bootloader();
    std::string username;
    std::cin.ignore(256, '\n');
    std::cout << "Enter your username: ";
    getline(std::cin, username);
    executeCommand("passwd " + username);
    std::cout << "You have finished installation of minimal Gentoo.\nPress enter to reboot system.";
    getch();
    executeCommand("exit && reboot");
}