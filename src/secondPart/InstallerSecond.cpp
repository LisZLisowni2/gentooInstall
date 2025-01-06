#include "InstallerSecond.h"
#include "utils.h"
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
}