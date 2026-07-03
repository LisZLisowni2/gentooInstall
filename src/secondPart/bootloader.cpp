#include "InstallerSecond.hpp"
#include "utils.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <map>
#include <vector>

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/app.hpp>
#include <ftxui/component/event.hpp>

using namespace ftxui;

std::string InstallerSecond::diskSelection() {
    executeCommand("(lsblk -rd | awk '!/NAME/ { print $1 \" \" $4 }') > /tmp/devices.tmp");
    std::ifstream deviceFile("/tmp/devices.tmp");
    std::string line;
    std::vector<std::string> options = {};
    
    while (getline(deviceFile, line)) {
        if (!line.empty() && line[line.length() - 1] == '\n') {
            line.erase(line.length() - 1);
        }

        options.push_back(line);
    }

    options.push_back("Continue");

    auto screen = App::Fullscreen();

    int selected = 0;

    auto menu = Menu(&options, &selected);

    auto layout = Renderer(menu, [&] {
        return vbox({
            text(" GentooInstall ") | bold | center | border,
            separator(),
            text("Use UP/DOWN arrow keys to navigate. Press ENTER to select"),
            separator(),
            menu->Render() | vscroll_indicator | frame | border | size(HEIGHT, LESS_THAN, 15),
            separator(),
            vbox({
                text(" Select your main disk ") | bold,
            }) | border,
        });
    });

    auto inputHandler = CatchEvent(layout, [&](Event event) {
        if (event == Event::Return) {
            screen.ExitLoopClosure()();
            
            return true;
        }

        return false;
    });

    screen.Loop(inputHandler);

    return options[selected];
}

void InstallerSecond::bootloader() {
    if (isEFI) executeCommand("echo \"sys-boot/grub efi\" > /etc/portage/package.use/grub");
    installPackages("sys-boot/grub");
    if (isEFI) executeCommand("grub-install --target=x86_64-efi --efi-directory=/boot/efi --bootloader-id=GENTOO");
    else  {
        std::string disk = diskSelection();
        executeCommand("grub-install --target=i386-pc " + disk); 
    }
    executeCommand("grub-mkconfig -o /boot/grub/grub.cfg");
    executeCommand("passwd");
}