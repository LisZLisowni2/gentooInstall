#include "InstallerFirst.h"
#include "startMenu.cpp"
#include "networkConfig.cpp"
#include "format.cpp"
#include "disk.cpp"
#include "mount.cpp"
#include <iostream>
#include <stdexcept>
#include <cstdlib>

InstallerFirst::InstallerFirst() {
    std::cout << "Gentoo Installer Initialized.\n"; 
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
}