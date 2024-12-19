#include "InstallerFirst.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>

void InstallerFirst::networkConfig() {
    std::vector<OptionMenu> options = {
        OptionMenu("Test the Internet", 0),
        OptionMenu("Configure Wifi", 1),
        OptionMenu("Next", -1),
    };
    while (true) {
        int key = selectMenu(options, "Internet configuration", "Internet is required to continue, because we have to in future sync with emerge database to install or update packages.");
        clearScreen();
        switch (key) {
            case -1:
                return;
                break;
            case 0:
                executeCommand("ping google.com -c 5");
                break;
            case 1:
                std::cout << "List of available network interfaces\n\n";
                executeCommand("ip link");
                std::cout << "\nSelect your interface";
                std::string interface;
                std::cin >> interface;
                break;
        }
        std::cout << "\n\nPress any key to continue.\n";
        getch();
    }
}