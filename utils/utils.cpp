#include "utils.h"
#include "Option.h"
#include <termcolor/termcolor.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include <termios.h>
#include <unistd.h>

#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_RIGHT 67
#define KEY_LEFT 68

int executeCommand(const std::string& command) {
    int result = std::system(command.c_str());
    if (result != 0) {
        std::cout << "Command failed: " << command << " | Error code: " << result << "\n";
    }
    return result;
}

char getch() {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Disable bufforing and echoing
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    if (ch == '\x1b') {
        if (getchar() == '[') {
            switch (getchar()) {
                case 'A': ch = KEY_UP; break;
                case 'B': ch = KEY_DOWN; break;
                case 'C': ch = KEY_RIGHT; break;
                case 'D': ch = KEY_LEFT; break;
                default: ch = '\x1b'; break;
            }
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void clearScreen() {
    std::cout << "\033[2J\033[H";
}

int selectMenu(const std::vector<OptionMenu>& options, const std::string& title, const std::string& description) {
    int selected = 0;
    int optionsLength = options.size();
    while (true) {
        std::cout << title << "\n";
        std::cout << description << "\n\n\n";
        int i = 0;
        for (auto option : options) {
            if (i == selected) {
                std::cout << " " << termcolor::on_bright_white << termcolor::dark << option.title << termcolor::reset << "\n";
            } else {
                std::cout << " " << option.title << "\n";
            }
            i++;
        }
        int key = getch();
        clearScreen();
        switch (key) {
            case KEY_UP:
                if (selected > 0) selected--;
                break;
            case KEY_DOWN:
                if (selected < optionsLength - 1) selected++;
                break;
            case KEY_RIGHT:
                clearScreen();
                return options[selected].actionID;
                break;
        }
    }
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