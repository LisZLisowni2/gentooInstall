#include "InstallerSecond.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <map>
#include <vector>

void InstallerSecond::makeMain() {
    std::vector<OptionMenu> options = {
        OptionMenu("Edit file", 0),
        OptionMenu("Next", -1),
    };
    while (true) {
        clearScreen();
        int key = selectMenu(options, "make.conf file", "Portage is a Gentoo's package manager that compiles every package from source. It is excellent for customizing your experience for your needs but it costs... time. Installing firefox package can take hours, so the best idea is the optimize the make.conf file. There are some rules for that, I will present it in shorter form:\n- COMMON_FLAGS flag - Defines flags for compilation commands. The best settings is -march=native, -O2 and -pipe. -march set the target architecture name, option native as that tells the compiller to select the target architecture installed of the current system. -O2 specifies gcc optimization class flag. The best set it to -O2. The last is -pipe which tells the compiler to use more memory. If you have a low memory and gcc crashing, delete this option.\n- MAKEOPTS flag - Defines how many parallel jobs can be do at same time. I recommends (like on official handbook) to set how many CPU threads do you have or RAM divided by 2. Example: 4 thread CPU = -j4, 2 thread CPU = -j2, 4 GB RAM = -j2. Definition: -jN, where N is number of available parallel jobs.\n- EMERGE_DEFAULT_OPTS flag - Defines default command for emerge command. Recommendations: --jobs=N (number of jobs you set in MAKEOPTS) --load-average=N (it can have more value, your decision).\n- ACCEPT_LICENSE flag - Defines licesne you want use. Example if you want only free software set e.g. -* @FREE. If you want exclude some licenses set e.g. * -@EULA. If you want use any license set *\n- FEATURES flag - Defines additional modules for installation. Example parallel-install enables compiling and downloading next packages at same time. There are many features, check the Internet for more information.\n- USE flag - Include or exclude packages to install additional packages. Works like the ACCEPT_LICENSE flag but it hasn't have * option (or exist but is bad idea, I don't know).\n\nThere aren't all flags but the important one.\n\nWARNING! THIS STEP ISN'T AUTOMATIC, YOU HAVE TO MANUALLY SET FLAGS!");
        std::cout << "\n";
        switch (key) {
            case -1:
                return;
                break;
            case 0:
                executeCommand("nano /etc/portage/make.conf");
                break;
        }
        std::cout << "\n\nPress any key to continue.\n";
        getch();
        return;
    }
}