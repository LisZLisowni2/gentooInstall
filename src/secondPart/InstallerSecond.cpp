#include "InstallerSecond.h"
#include "utils.h"
#include "makeMain.cpp"
#include "profile.cpp"
#include "kernel.cpp"
#include "networkConfig.cpp"
#include <iostream>
#include <stdexcept>
#include <cstdlib>

InstallerSecond::InstallerSecond() {
    std::cout << "Gentoo Installer Initialized.\n"; 
}

InstallerSecond::~InstallerSecond() {
    std::cout << "Gentoo Installer Completed.\n"; 
}

void InstallerSecond::install() {
    std::string hostname;
    executeCommand("emerge-webrsync");
    makeMain();
    profile();
    executeCommand("emerge --deep --update --newuse --verbose @world");
    kernelConfig();
    kernelInstall();
    executeCommand("emerge sys-fs/genfstab");
    executeCommand("genfstab -U / >> /etc/fstab");
    clearScreen();
    networkConfig();
    std::cout << "Enter your hostname: ";
    std::cin >> hostname;
    executeCommand("echo \"" + hostname + "\" > /etc/hostname");
    executeCommand("echo \"127.0.0.1\t" + hostname + "\" > /etc/hosts");
    executeCommand("echo \"sys-boot/grub efi\" >> /etc/portage/portage.use && emerge sys-boot/grub");
    executeCommand("grub-install --target=x86_64-efi --efi-directory=/boot/efi --bootloader-id=GENTOO");
    executeCommand("grub-mkconfig -o /boot/grub/grub.cfg");
    std::cout << "You have finished installation of Gentoo... the most basic parts of it.\nThere are some other thing to do like root password, add local users, etc..\nIn future this Installer grants more interactive actions.\nIf you finish the work, exit the chrooted mode and reboot.";
}