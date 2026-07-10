#include "utils.hpp"
#include <iostream>
#include <cstdlib>
#include <string>
#include <stdexcept>

int executeCommand(const std::string& command) {
    int result = std::system(command.c_str());
    if (result != 0) {
        std::cout << "Command failed: " << command << " | Error code: " << result << "\n";
        getch();
    }
    return result;
}

void installPackages(const std::string& packages) {
    int code = executeCommand("emerge --autounmask-continue " + packages);
    // Alternatively, use that command to check error code in logs
    // tail -n 4 /var/log/emerge.log > /tmp/emerge.log
    // cat /tmp/emerge.log | head -n 3 | tail -n 1 | awk '{ print $7 }' | cut -c2
    if (code == 0) return;
    
    std::cout << "You encountered emerge error. There are some common issues and how to resolve:\n";
    std::cout << "- Masked packages (USE changes) => Script set --autounmask-continue flag for automatic unmask masked packages. [...]";
    std::cout << "- The ebuild selected to satisfy '...' has unmet requirements => Exit the script by Ctrl+C shortcut, then open the make.conf file by `nano /etc/portage/make.conf` command. The error is about USE settings that unable to install the package. Maybe some USE flags are wrong defined.\n";
}