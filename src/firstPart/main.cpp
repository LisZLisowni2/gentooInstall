#include "InstallerFirst.h"
#include "utils.h"
#include <cpr/cpr.h>
#include <iostream>

int main() {
    clearScreen();
    InstallerFirst installer;
    installer.install();
    return 0;
}