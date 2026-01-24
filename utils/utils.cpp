#include "utils.hpp"
#include "Option.hpp"
#include <sys/ioctl.h>
#include <termcolor/termcolor.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include <termios.h>
#include <unistd.h>
#include <regex>

#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_RIGHT 67
#define KEY_LEFT 68
#define KEY_H 72

std::pair<int, int> getTerminalSize() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return {w.ws_row, w.ws_col};
}

int executeCommand(const std::string& command) {
    int result = std::system(command.c_str());
    if (result != 0) {
        std::cout << "Command failed: " << command << " | Error code: " << result << "\n";
        getch();
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
    } else {
        switch(ch) {
            case '\n': ch = KEY_RIGHT; break;
            case 'h': ch = KEY_H; break;
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void clearScreen() {
    std::cout << "\033[2J\033[H";
}

template <typename ValueType>
int selectMenu(const std::vector<OptionMenu<ValueType>>& options, const std::string& title, const std::string& description) {
    int selected = 0;
    int optionsLength = options.size();
    int topVisible = 0;

    auto [terminalRows, terminalCols] = getTerminalSize();

    int reservedLines = 12;

    int maximumVisibleItems = terminalRows - reservedLines;

    if (maximumVisibleItems < 3) {
        maximumVisibleItems = 3;
    }

    while (true) {
        std::cout << title << "\n";
        std::cout << description << "\n\n\n";
        
        if (selected >= topVisible + maximumVisibleItems) {
            topVisible = selected - maximumVisibleItems + 1;
        } else if (selected < topVisible) {
            topVisible = selected;
        }

        if (topVisible < 0) {
            topVisible = 0;
        }
        if (topVisible > optionsLength - maximumVisibleItems) {
            topVisible = std::max(0, optionsLength - maximumVisibleItems);
        }
        if (topVisible > 0) {
            std::cout << " ↑ (" << topVisible << " more above)\n";
        }

        int endVisible = std::min(topVisible + maximumVisibleItems, optionsLength);
        for (int i = topVisible; i < endVisible; i++) {
            std::string prefix = " ";

            if (optionsLength > 10) {
                prefix = "[" + std::to_string(i + 1) + "]";
            }

            if (i == selected) {
                std::cout << " " << termcolor::on_bright_white << termcolor::dark << options[i].title << termcolor::reset << "\n";
            } else {
                std::cout << " " << options[i].title << "\n";
            }
        }

        int hiddenBelow = optionsLength - (topVisible + maximumVisibleItems);
        if (hiddenBelow > 0) {
            std::cout << " ↓ (" << hiddenBelow << " more below)\n";
        }

        std::cout << "\nPosition: " << (selected + 1) << "/" << optionsLength;
        if (optionsLength > maximumVisibleItems) {
            std::cout << " | Use ↑↓ to scroll, → to select";
        }
        std::cout << "\n";

        int key = getch();
        clearScreen();
        switch (key) {
            case KEY_UP:
                if (selected > 0) selected--;
                else selected = optionsLength - 1;
                break;
            case KEY_DOWN:
                if (selected < optionsLength - 1) selected++;
                else selected = 0;
                break;
            case KEY_RIGHT:
                clearScreen();
                return options[selected].actionID;
                break;
            case KEY_H:
                clearScreen();
                helpBox();
                break;
        }
    }
}

template int selectMenu<std::string>(const std::vector<OptionMenu<std::string>>&, const std::string&, const std::string&);
template int selectMenu<int>(const std::vector<OptionMenu<int>>&, const std::string&, const std::string&);

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

void helpBox() {
    int max_width = 80;
	clearScreen();
	std::cout << termcolor::bright_red << "Help menu\n\n" << termcolor::reset;
    std::cout << termcolor::bright_cyan << "Controls:\n" << termcolor::reset;
    std::cout << "Right arrow/enter key - Select option\n";
    std::cout << "Top arrow key - Upper option\n";
    std::cout << "Down arrow key - Lower option\n\n";
    std::cout << "Press any key to continue...";
    getch();
    clearScreen();
}