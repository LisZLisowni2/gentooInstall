#include "InstallerSecond.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <map>
#include <vector>

void InstallerSecond::kernelConfig() {
    std::vector<OptionMenu> options = {
        OptionMenu("Install linux firmware", 0),
        OptionMenu("Install intel microcode", 1),
        OptionMenu("Install amd microcode", 2),
        OptionMenu("Install sof firmware", 3),
        OptionMenu("Next", -1),
    };
    while (true) {
        clearScreen();
        int key = selectMenu(options, "Kernel - Config", "We have system but we don't have kernel. Without kernel, system wouldn't run. There is two options, install precompiled kernel or configure it ourself. If you decided to configure kernel manually, follow the instructions on https://wiki.gentoo.org/wiki/Handbook:AMD64/Installation/Kernel, but wait. I again mention that, then you can configure your kernel. Before install kernel we have to add some firmwares that maybe we necessary for you, like drivers for wireless card, GPU or microcode for intel/amd CPU. I recommend for install all of it basing on your CPU. (sof-firmware -> Drivers for sound drivers)");
        int id;
        std::cout << "\n";
        switch (key) {
            case -1:
                return;
                break;
            case 0:
                executeCommand("emerge sys-kernel/linux-firmware");
                break;
            case 1:
                executeCommand("emerge sys-firmware/intel-microcode");
                break;
            case 2:
                executeCommand("emerge sys-firmware/amd-microcode");
                break;
            case 3:
                executeCommand("emerge sys-firmware/sof-firmware");
                break;
        }
        std::cout << "\n\nPress any key to continue.\n";
        getch();
    }
}

void InstallerSecond::kernelInstall() {
    std::vector<OptionMenu> options = {
        OptionMenu("Install precompiled kernel", 0),
        OptionMenu("Install and configure kernel", 1)
    };
    while (true) {
        clearScreen();
        int key = selectMenu(options, "Kernel - Install", "After basic configuration we can install kernel. There are some options, I mention only two.\n1) Precompiled - The fastest and the easiest way. Installs the precompiled kernel that doesn't need to config.\n2) Own kernel - For more advanced users there is option for them. Second option give free hand to configure own kernel for your specific requirements.");
        int id;
        std::cout << "\n";
        switch (key) {
            case 0:
                installPackages("gentoo-kernel-bin");
                break;
            case 1:
                std::cout << "This option work in progress\n";
                break;
        }
        std::cout << "\n\nPress any key to continue.\n";
        getch();
        return;
    }
}
