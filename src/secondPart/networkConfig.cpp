#include "InstallerSecond.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>

void InstallerSecond::networkConfig() {
    clearScreen();
    std::string hostname;
    std::cout << "Enter your hostname: ";
    std::cin >> hostname;
    executeCommand("echo \"" + hostname + "\" > /etc/hostname");
    executeCommand("echo \"127.0.0.1\t" + hostname + "\" >> /etc/hosts");
    executeCommand("echo \"sys-boot/grub efi\" > /etc/portage/portage.use/grub");
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
                if (isOpenRC) executeCommand("rc-update add dhcpcd default");
                else executeCommand("systemctl enable dhcpcd");
                return;
                break;
            case 1:
                installPackages("net-misc/networkmanager");
                if (isOpenRC) executeCommand("rc-update add NetworkManager default");
                else executeCommand("systemctl enable NetworkManager");
                return;
                break;
        }
        std::cout << "\n\nPress any key to continue.\n";
        getch();
    }
}