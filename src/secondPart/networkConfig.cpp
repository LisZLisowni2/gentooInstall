#include "InstallerSecond.hpp"
#include "utils.hpp"
#include <limits>
#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/app.hpp>
#include <ftxui/component/event.hpp>

using namespace ftxui;

void InstallerSecond::hostnameSet() {
    std::string hostname;
    std::string placeholder = "Type hostname for your system...";

    auto input = Input(&hostname, &placeholder);

    auto layout = Renderer(input, [&] {
        return vbox({
            text(" GentooInstall "),
            separator(),
            window(text(""), input->Render()),
        });
    });

    auto screen = App::Fullscreen();

    auto inputHandler = CatchEvent(layout, [&](Event event) {
        if (event == Event::Return) {
            screen.ExitLoopClosure()();
            return true;
        }

        if (event == Event::Backspace && hostname.empty()) {
            return true;
        }

        return false;
    });

    screen.Loop(inputHandler);

    executeCommand("echo \"" + hostname + "\" > /etc/hostname");
    executeCommand("echo \"127.0.0.1\t" + hostname + "\" >> /etc/hosts");
}

void InstallerSecond::networkConfig() { 
    hostnameSet();

    std::vector<std::string> options = {
        "Use DHCPCD",
        "Use NetworkManager",
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
            installPackages("net-misc/dhcpcd");        
            if (isOpenRC) executeCommand("rc-update add dhcpcd default");
            else executeCommand("systemctl enable dhcpcd");
            break;
        case 1:
            installPackages("net-misc/networkmanager");
            if (isOpenRC) executeCommand("rc-update add NetworkManager default");
            else executeCommand("systemctl enable NetworkManager");
            break;
    }
}