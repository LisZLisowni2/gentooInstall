#include "InstallerFirst.h"
#include "termcolor/termcolor.hpp"
#include "utils.h"
#include <iostream>
#include <stdexcept>
#include <cstdlib>

void InstallerFirst::startMenu() {
    int max_width = 80;
	clearScreen();
	std::cout << " Welcome to ";
    std::cout << termcolor::bright_cyan << "GentooInstaller " << termcolor::reset;
    std::cout << " script!\n\n";
    std::cout << "This is the prototype of installer like archinstall.\nIt isn't fully automated, during installation you have to configure yourself but I guarantee you\nI will explain you that you can be yourself.\nGentoo is Linux distribution focused to customization, you can precised to use systemd or openrc,\nconfigure each programe for your needs and so on.\nIf you intercept an error during script, create pull request and describe your problem.\nIt will help us to improve this script.\n\n";
	std::cout << "If you are ready, click any key to continue\n";
    getch();
}