#include "InstallerSecond.h"
#include "utils.h"
#include "makeMain.cpp"
#include "profile.cpp"
#include "kernel.cpp"
#include "networkConfig.cpp"
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <fstream>

InstallerSecond::InstallerSecond() {
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

InstallerSecond::~InstallerSecond() {
    std::cout << "Gentoo Installer Completed.\n"; 
}

void InstallerSecond::install() {
    std::string hostname;
    std::string username;
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
    std::cout << "Enter your hostname: ";
    std::cin >> hostname;
    executeCommand("echo \"" + hostname + "\" > /etc/hostname");
    executeCommand("echo \"127.0.0.1\t" + hostname + "\" >> /etc/hosts");
    executeCommand("echo \"sys-boot/grub efi\" > /etc/portage/portage.use/grub");
    installPackages("sys-boot/grub");
    executeCommand("grub-install --target=x86_64-efi --efi-directory=/boot/efi --bootloader-id=GENTOO");
    executeCommand("grub-mkconfig -o /boot/grub/grub.cfg");
    executeCommand("passwd");
    std::cin.ignore(256, '\n');
    std::cout << "Enter your username: ";
    getline(std::cin, username);
    executeCommand("passwd " + username);
    std::cout << "You have finished installation of minimal Gentoo.\nPress enter to reboot system.";
    getch();
    executeCommand("exit && reboot");
}