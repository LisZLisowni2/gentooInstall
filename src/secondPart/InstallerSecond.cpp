#include "InstallerSecond.h"
#include "utils.h"
#include "makeMain.cpp"
#include "profile.cpp"
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
    executeCommand("emerge-webrsync");
    makeMain();
    profile();
    executeCommand("emerge --deep --update --newuse --verbose @world");
}