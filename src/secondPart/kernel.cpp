#include "InstallerSecond.hpp"
#include "utils.hpp"
#include <string>
#include <iostream>
#include <stdexcept>
#include <map>
#include <vector>
#include <fstream>

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/app.hpp>
#include <ftxui/component/event.hpp>

using namespace ftxui;

void InstallerSecond::kernelConfig() {
    clearScreen();
    installPackages("sys-kernel/linux-firmware");
    executeCommand("if cat /proc/cpuinfo | grep -i intel >/dev/null; then emerge --autounmask-continue sys-firmware/intel-microcode sys-firmware/sof-firmware; fi");
}

void InstallerSecond::kernelInstall() {
    executeCommand("echo \"sys-kernel/installkernel dracut\" > /etc/portage/package.use/installkernel");
    std::vector<std::string> options = {
        "Install precompiled kernel",
        "Install and configure kernel (For Advanced Users)",
    };

    int selected = 0;

    auto screen = App::Fullscreen();

    auto menu = Menu(&options, &selected);

    auto layout = Renderer(menu, [&] {
        return vbox({
            text(" GentooInstall ") | bold | center | border,
            separator(),
            text("Use UP/DOWN arrow keys to navigate. Press ENTER to select"),
            separator(),
            menu->Render() | vscroll_indicator | frame | border | size(HEIGHT, LESS_THAN, 15),
        });
    });

    auto inputHandler = CatchEvent(layout, [&](Event event) {
        if(event == Event::Return) {
            screen.ExitLoopClosure()();
            return true;
        }

        return false;
    });

    screen.Loop(inputHandler);

    switch (selected) {
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
    return;
}

void InstallerSecond::kernelCompile() {
    std::vector<std::string> options = {
        "Read the lspci file (your PC's specs)",
        "Config kernel",
        "Compile", 
    };

    int selected = 0;

    auto screen = App::Fullscreen();

    auto menu = Menu(&options, &selected);

    auto layout = Renderer(menu, [&] {
        return vbox({
            text(" GentooInstall ") | bold | center | border,
            separator(),
            text("Use UP/DOWN arrow keys to navigate. Press ENTER to select"),
            separator(),
            menu->Render() | vscroll_indicator | frame | border | size(HEIGHT, LESS_THAN, 15),
        });
    });

    auto inputHandler = CatchEvent(layout, [&](Event event) {
        if(event == Event::Return) {
            screen.ExitLoopClosure()();
            return true;
        }

        return false;
    });
    
    while (true) {
        screen.Loop(inputHandler);
        clearScreen();
        
        switch (selected) {
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
    }
}