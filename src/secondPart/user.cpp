#include "InstallerSecond.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <map>
#include <vector>

void InstallerSecond::userCreation() {
    std::string username;
    std::cin.ignore(256, '\n');
    std::cout << "Enter your username: ";
    getline(std::cin, username);
    executeCommand("useradd -mG wheel,audio,video,usb,cdrom -s /bin/bash " + username);
    executeCommand("passwd " + username);
}