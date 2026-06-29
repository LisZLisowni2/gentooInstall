#include "InstallerFirst.hpp"
#include "utils.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <map>

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/app.hpp>
#include <ftxui/component/event.hpp>

using namespace ftxui;

enum Formats {
    ext4 = 0,
    fat32 = 1,
    ntfs = 2,
    btrfs = 3,
    xfs = 4,
    exfat = 5,
    swap = 6,
};

std::string InstallerFirst::partitionSelection() {
    executeCommand("(lsblk -rn | grep 'part' | awk '{ print $1 \" \" $4 }') > /tmp/partitions.tmp");
    std::ifstream partitionsFile("/tmp/partitions.tmp");
    std::string line;
    std::vector<std::string> options = {};
    std::string lineValue;
    int index = 0;
    while (getline(partitionsFile, line)) {
        if (!line.empty() && line[line.length() - 1] == '\n') {
            line.erase(line.length() - 1);
        }

        options.push_back(line);
        index++;
    }

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
            separator(),
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

    return "/dev/" + options[selected].substr(0, options[selected].find(' '));
}

std::string InstallerFirst::formatSelection() {
    std::vector<std::string> options = {
        "swap",
        "ext4",
        "fat32",
        "ntfs",
        "btrfs",
        "zfs",
        "exfat"
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
        if (event == Event::Return) {
            screen.ExitLoopClosure()();
            return true;
        }

        return false;
    });

    screen.Loop(inputHandler);

    return options[selected];
}

void InstallerFirst::formatPartition(std::string& format, std::string& partition) {
    std::map<std::string, Formats> formatsMap = {
        {"ext4", ext4},
        {"fat32", fat32},
        {"ntfs", ntfs},
        {"btrfs", btrfs},
        {"xfs", xfs},
        {"exfat", exfat},
    };

    switch (formatsMap[format]) {
        case Formats::swap:
            executeCommand("mkswap " + partition);
            break;
        case Formats::fat32:
            executeCommand("mkfs.fat -F 32 " + partition);
            break;
        case Formats::ntfs:
            executeCommand("mkfs.nsfat " + partition);
            break;
        case Formats::btrfs:
            executeCommand("mkfs.btrfs " + partition);
            break;
        case Formats::xfs:
            executeCommand("mkfs.xfs " + partition);
            break;
        case Formats::exfat:
            executeCommand("mkfs.exfat " + partition);
            break;
    }
}

void InstallerFirst::format() {
    std::vector<std::string> options = {
        "Format partition",
        "Continue",
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
        if (options[selected] == "Continue") {
            break;
        } else {
            std::string partition = partitionSelection();
            std::string format = formatSelection();
            formatPartition(partition, format);
        }
    }
}