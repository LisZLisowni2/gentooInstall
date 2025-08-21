#include "InstallerFirst.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>

std::string InstallerFirst::interfaceSelection() {
    executeCommand("(ip link | awk '{ print $2 }' | sed 's/:/*/') > /tmp/interfaces.tmp");
    std::ifstream interfacesFile("/tmp/interfaces.tmp");
    std::string line;
    std::vector<OptionMenu> options = {};
    int index = 0;
    while (getline(interfacesFile, line)) {
        if (!line.empty() && line[line.length() - 1] == '\n') {
            line.erase(line.length() - 1);
        }

        if (line.at(line.length() - 1) == '*') { 
            line.erase(line.length() - 1);
            options.push_back(OptionMenu(line, index));
            index++;
        }
    }

    clearScreen();
    int key = selectMenu(options, "List of available network interfaces", "To configure Wifi choose correct internet interface, most often wireless interface starts with wl");
    std::cout << "\n";
 
    return options[key].title;
}

std::string InstallerFirst::wifiSelection() {
    while (true) {
        executeCommand("(iwlist wlan0 scan | awk -F ':' '/ESSID:/ {gsub(/\"/,\"\",$2); if(!seen[$2]++) print $2}' | sed 's/\"//; s/\(.*\)\"$/\1/') > /tmp/wifi.tmp");
        std::ifstream interfacesFile("/tmp/interfaces.tmp");
        std::string line;
        std::vector<OptionMenu> options = {};
        int index = 0;
        while (getline(interfacesFile, line)) {
            if (!line.empty() && line[line.length() - 1] == '\n') {
                line.erase(line.length() - 1);
            }
    
            options.push_back(OptionMenu(line, index));
            index++;
        }
    
        options.push_back(OptionMenu("Refresh", -1)); // -1 -> Default
    
        clearScreen();
        int key = selectMenu(options, "List of available wifi access points", "To configure Wifi choose correct access point");
        std::cout << "\n";
    
        if (key == -1) {
            return options[key].title;
        }
    }
}

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
                interface = interfaceSelection();
                executeCommand("ip link set " + interface + " up");
                ssid = wifiSelection();
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