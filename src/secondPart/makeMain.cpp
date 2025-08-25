#include "InstallerSecond.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <map>
#include <vector>

void InstallerSecond::makeMain() {
    bool autoUsed = false;
    std::vector<OptionMenu<std::string>> options = {
        OptionMenu("Edit file", 0),
        OptionMenu("Make.conf describe file", 1), // TODO: Download from another github repo make.conf describe file
        OptionMenu("Automatic", 2), 
        OptionMenu("Next", -1),
    };
    while (true) {
        clearScreen();
        int key = selectMenu(options, "make.conf file", "Portage is a Gentoo's package manager that compiles every package from source. It is excellent for customizing your experience for your needs but it costs... time. Installing firefox package can take hours, so the best idea is the optimize the make.conf file. In version v2.1.1 I will create file that describe make.conf options. If you are new user I recommend 'Automatic' option.");
        std::cout << "\n";
        switch (key) {
            case -1:
                return;
                break;
            case 0:
                executeCommand("nano /etc/portage/make.conf");
                break;
            case 1:
                if (autoUsed) {
                    break;
                }
                executeCommand("cp /etc/portage/make.conf /etc/portage/make.conf.old");
                executeCommand("echo 'MAKEOPTS=\"-j'$(nproc)' -l'$(nproc)'\"' >> /etc/portage/make.conf");
                executeCommand("echo 'EMERGE_DEFAULT_OPTS=\"--jobs='$(nproc)' --load-average='$(nproc)'\"' >> /etc/portage/make.conf");
                executeCommand("echo 'FEATURES=\"parallel-fetch\"' >> /etc/portage/make.conf");
                executeCommand("echo 'COMMON_FLAGS=\"${COMMON_FLAGS} -march=native\"' >> /etc/portage/make.conf");
                executeCommand("echo 'ACCEPT_LICENSE=\"*\"' >> /etc/portage/make.conf");
                executeCommand("echo 'PORTDIR=\"/var/db/repos/gentoo\"' >> /etc/portage/make.conf");
                executeCommand("echo 'DISTDIR=\"/var/cache/distfiles\"' >> /etc/portage/make.conf");
                executeCommand("echo 'PKGDIR=\"/var/cache/binpkgs\"' >> /etc/portage/make.conf");
                autoUsed = true;
                break;
        }
        std::cout << "\n\nPress any key to continue.\n";
        getch();
    }
}
