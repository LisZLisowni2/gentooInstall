#include "InstallerSecond.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>

void InstallerSecond::networkConfig() {
    std::vector<OptionMenu> options = {
        OptionMenu("Use DHCPCD", 0),
        OptionMenu("Use NetworkManager", 1),
    };
    while (true) {
        clearScreen();
        int key = selectMenu(options, "Network config on system", "We have already Internet, but not in the system. If we reboot now, the internet doesn't work. Install one of these, personally I prefer NetworkManager, but your decision. After that you will not have any option menu.");
        std::cout << "\n";
        switch (key) {
            case 0:
                installPackages("net-misc/dhcpcd");
                executeCommand("rc-update add dhcpcd default");
                return;
                break;
            case 1:
                installPackages("net-misc/networkmanager");
                executeCommand("rc-update add NetworkManager default");
                return;
                break;
        }
        std::cout << "\n\nPress any key to continue.\n";
        getch();
    }
}