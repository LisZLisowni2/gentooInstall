#include "InstallerFirst.hpp"
#include "utils.hpp"
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

std::string InstallerFirst::mountPathInput() {
    std::string path = "";
    std::string placeholder = "Type path where you want mount the partition...";

    auto inputField = Input(&path, &placeholder);

    auto layout = Renderer(inputField, [&] {
        return vbox({
            text(" GentooInstall "),
            separator(),
            window(text(""), inputField->Render()),
        });
    });

    auto screen = App::Fullscreen();

    auto inputHandler = CatchEvent(layout, [&](Event event) {
        if (event == Event::Return) {
            screen.ExitLoopClosure()();
            return true;
        }

        if (event == Event::Backspace && path.empty()) {
            return true;
        }

        return false;
    });

    screen.Loop(inputHandler);

    return path;
}

void InstallerFirst::mount() {
    std::vector<std::string> options = {
        "Mount root partition",
        "Mount boot partition",
        "Mount other partition",
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
            switch (selected) {
                case 0:
                    executeCommand("sudo mount " + partition + " /mnt/gentoo");
                    break;
                case 1:
                    if (isEFI) executeCommand("sudo mount --mkdir " + partition + " /mnt/gentoo/boot/efi");
                    else executeCommand("sudo mount --mkdir " + partition + " /mnt/gentoo/boot/");
                    break;
                case 2:
                    std::string path = mountPathInput();
                    executeCommand("sudo mount --mkdir " + partition + " " + path);
                    break;
            }
        }
    }
}