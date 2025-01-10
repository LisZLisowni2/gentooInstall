#include "InstallerFirst.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <map>
const std::string DATETIME = "20250105T170325Z";

void InstallerFirst::tarball() {
    std::vector<OptionMenu> options = {
        OptionMenu("OpenRC", 0),
        OptionMenu("Systemd", 1),
        OptionMenu("OpenRC | desktop profile", 2),
        OptionMenu("Systemd | desktop profile", 3),
    };
    while (true) {
        clearScreen();
        int key = selectMenu(options, "Stage 3 tarball", "Stage 3 file contains basic structure of system like /usr /bin or /tmp directories. Gentoo default works on OpenRC. Automatically install the tarball file :D");
        std::cout << "\n";
        switch (key) {
            case 0:
                executeCommand("wget https://distfiles.gentoo.org/releases/amd64/autobuilds/" + DATETIME + "/stage3-amd64-openrc-" + DATETIME + ".tar.xz -P /mnt/gentoo");
                executeCommand("cd /mnt/gentoo && tar xpvf stage3-amd64-openrc-" + DATETIME + ".tar.xz --xattrs-include='*.*' --numeric-owner");
                break;
            case 1:
                executeCommand("wget https://distfiles.gentoo.org/releases/amd64/autobuilds/" + DATETIME + "/stage3-amd64-systemd-" + DATETIME + ".tar.xz -P /mnt/gentoo");
                executeCommand("cd /mnt/gentoo && tar xpvf stage3-amd64-systemd-" + DATETIME + ".tar.xz --xattrs-include='*.*' --numeric-owner");
                break;
            case 2:
                executeCommand("wget https://distfiles.gentoo.org/releases/amd64/autobuilds/" + DATETIME + "/stage3-amd64-desktop-openrc-" + DATETIME + ".tar.xz -P /mnt/gentoo");
                executeCommand("cd /mnt/gentoo && tar xpvf stage3-amd64-desktop-openrc-" + DATETIME + ".tar.xz --xattrs-include='*.*' --numeric-owner");
                break;
            case 3:
                executeCommand("wget https://distfiles.gentoo.org/releases/amd64/autobuilds/" + DATETIME + "/stage3-amd64-desktop-systemd-" + DATETIME + ".tar.xz -P /mnt/gentoo");
                executeCommand("cd /mnt/gentoo && tar xpvf stage3-amd64-desktop-systemd-" + DATETIME + ".tar.xz --xattrs-include='*.*' --numeric-owner");
                break;
        }
        std::cout << "\n\nPress any key to continue.\n";
        getch();
        return;
    }
}
