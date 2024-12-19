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

void executeCommand(const std::string& command) {
    int result = std::system(command.c_str());
    if (result != 0) {
        throw std::runtime_error("Command failed: " + command);
    }
}

char getch() {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Disable bufforing and echoing
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
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
        clearScreen();
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
        switch (key) {
            case KEY_UP:
                if (selected > 0) selected--;
                break;
            case KEY_DOWN:
                if (selected < optionsLength - 1) selected++;
                break;
            case KEY_RIGHT:
                return options[selected].actionID;
                break;
        }
    }
}