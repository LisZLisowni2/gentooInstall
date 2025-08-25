#include "InstallerFirst.h"
#include "utils.h"
#include <iostream>

int main() {
    clearScreen();
    InstallerFirst installer;
    installer.install();
    executeCommand("reboot");
    return 0;
}