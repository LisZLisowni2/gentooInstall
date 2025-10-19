#include "InstallerSecond.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <map>
#include <vector>
#include <fstream>

void InstallerSecond::kernelConfig() {
    clearScreen();
    installPackages("sys-kernel/linux-firmware");
    executeCommand("if cat /proc/cpuinfo | grep -i intel >/dev/null; then emerge --autounmask-continue sys-firmware/intel-microcode sys-firmware/sof-firmware; fi");
}

void InstallerSecond::kernelInstall() {
    executeCommand("echo \"sys-kernel/installkernel dracut\" > /etc/portage/package.use/installkernel");
    std::vector<OptionMenu<std::string>> options = {
        OptionMenu("Install precompiled kernel", 0),
        OptionMenu("Install and configure kernel (For Advanced Users)", 1)
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
                installPackages("sys-kernel/gentoo-sources sys-kernel/installkernel sys-apps/pciutils");
                executeCommand("lspci > /tmp/lspci.tmp");
                std::cout << "Preconfiguration\n";
                executeCommand("ln -sf /usr/src/linux-* /usr/src/linux");
                executeCommand("cd /usr/src/linux && make localmodconfig");
                kernelCompile();
                break;
        }
        std::cout << "\n\nPress any key to continue.\n";
        getch();
        return;
    }
}

void InstallerSecond::kernelCompile() {
    std::vector<OptionMenu<std::string>> options = {
        OptionMenu("Read the lspci file (your PC's specs)", 0),
        OptionMenu("Config kernel", 1),
        OptionMenu("Compile", 2)
    };
    while (true) {
        clearScreen();
        int key = selectMenu(options, "Kernel - Compilation", "It is important to correctly configure the kernel unless you don't want to run the OS. If you have no idea, what to do, check the official Gentoo Handbook");
        std::cout << "\n";
        switch (key) {
            case 0:
                executeCommand("less /tmp/lspci.tmp");
                break;
            case 1:
                executeCommand("cd /usr/src/linux && make nconfig");
                break;
            case 2:
                int res = executeCommand("cd /usr/src/linux && make -j$(nproc) && make -j$(nproc) modules_install && make -j$(nproc) install");
                if (res == 0) return;
                std::cout << "Something is wrong, retrying\n";
                executeCommand("cd /usr/src/linux && make clean");
                res = executeCommand("cd /usr/src/linux && make -j$(nproc) && make -j$(nproc) modules_install && make -j$(nproc) install");
                if (res == 0) return;
                std::cout << "Something wrong is in your kernel configuration. Check out the config and retry compilation";
                break;
        }
        std::cout << "\n\nPress any key to continue.\n";
        getch();
    }
}