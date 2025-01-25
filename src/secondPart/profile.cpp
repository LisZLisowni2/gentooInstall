#include "InstallerSecond.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <map>
#include <vector>

void InstallerSecond::profile() {
    std::vector<OptionMenu> options = {
        OptionMenu("All profiles", 0),
        OptionMenu("Set profile", 1),
        OptionMenu("Next", -1),
    };
    while (true) {
        clearScreen();
        int key = selectMenu(options, "Profile", "In Gentoo profiles defines the basic packages to be installed. The default profile depends on extracted tarball. If you want change profile select the second option");
        int id;
        std::cout << "\n";
        switch (key) {
            case -1:
                return;
                break;
            case 0:
                executeCommand("eselect profile list | less");
                break;
            case 1:
                executeCommand("eselect profile list");
                std::cout << "\n\n";
                std::cout << "Select the id: ";
                std::cin >> id;
                std::cout << "eselect profile set " << id << "\n"; // Debugging
                executeCommand("eselect profile set " + id); // FIXME: Error produced
                break;
        }
        std::cout << "\n\nPress any key to continue.\n";
        getch();
    }
}
