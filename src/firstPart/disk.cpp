#include "InstallerFirst.hpp"
#include "utils.hpp"
#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <fstream>

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/app.hpp>
#include <ftxui/component/event.hpp>

using namespace ftxui;

void InstallerFirst::partitions() {
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
                text(" Partition Management Guide ") | bold,
                text(" Ensure your target drive has the following layout for a standard UEFI system:"),
                text("  • /dev/sdX1 - EFI System Partition (esp) -> 512MB to 1GB (FAT32)"),
                text("  • /dev/sdX2 - Swap Partition            -> 2GB to 16GB (SWAP)"),
                text("  • /dev/sdX3 - Root Partition (/)         -> Remaining Space (ext4/btrfs)"),
                text(""),
                text(" Note: Select your root partition to begin installation, or choose 'Continue'.") | color(Color::Yellow),
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

    while (true) {
        screen.Loop(inputHandler);

        if (options[selected] != "Continue") {
            executeCommand("cfdisk /dev/" + options[selected].substr(0, options[selected].find(" ")));
        } else {
            break;
        }
    }
}