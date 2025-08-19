#include "InstallerFirst.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>

void InstallerFirst::networkConfig() {
    std::vector<OptionMenu> options = {
        OptionMenu("Test the Internet", 0),
        OptionMenu("Configure Wifi", 1),
        OptionMenu("Next", -1),
    };
    while (true) {
        clearScreen();
        int key = selectMenu(options, "Internet configuration", "Internet is required to continue, because we have to in future sync with emerge database to install or update packages.");
        std::cout << "\n";
        switch (key) {
            case -1:
                return;
                break;
            case 0:
                executeCommand("ping google.com -c 5");
                break;
            case 1:
                std::string interface;
                std::string ssid;
                std::string password;
                std::cout << "List of available network interfaces\n\n";
                executeCommand("(ip link | awk '{ print $2 }' | sed 's/:/*/') > /tmp/interfaces.tmp");
                std::cout << "\nSelect your interface";
                std::cin >> interface;
                executeCommand("ip link set " + interface + " up");
                std::cout << "\nAll available networks";
                executeCommand("iwlist " + interface + " scan | grep ESSID");
                std::cout << "\nInput SSID (Network name): ";
                std::cin >> ssid;
                std::cout << "\nInput password for your network: ";
                std::cin >> password;
                executeCommand("wpa_passphrase \"" + ssid + "\" \"" + password + "\" > /etc/wpa_supplicant.conf");
                executeCommand("wpa_supplicant -B -i " + interface + " -c /etc/wpa_supplicant.conf");
                executeCommand("dhcpcd " + interface);
                std::cout << "\nWifi configured";
                break;
        }
        std::cout << "\n\nPress any key to continue.\n";
        getch();
    }
}