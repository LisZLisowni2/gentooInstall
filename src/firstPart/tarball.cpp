#include "InstallerFirst.hpp"
#include "utils.hpp"
#include "requests.hpp"
#include <string>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <map>

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/app.hpp>

using namespace ftxui;

void InstallerFirst::tarball() {
    clearScreen();
    std::cout << "Reading the latest version of taball\n\n";
    std::string DATETIME = latestVersion(isARM);
    std::vector<std::string> options = {
        "OpenRC",
        "Systemd",
        "OpenRC | desktop profile",
        "Systemd | desktop profile",
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

    if (!isARM) {
        switch (selected) {
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
    } else {
        switch (selected) {
            case 0:
                executeCommand("wget https://distfiles.gentoo.org/releases/arm64/autobuilds/" + DATETIME + "/stage3-arm64-openrc-" + DATETIME + ".tar.xz -P /mnt/gentoo");
                executeCommand("cd /mnt/gentoo && tar xpvf stage3-arm64-openrc-" + DATETIME + ".tar.xz --xattrs-include='*.*' --numeric-owner");
                break;
            case 1:
                executeCommand("wget https://distfiles.gentoo.org/releases/arm64/autobuilds/" + DATETIME + "/stage3-arm64-systemd-" + DATETIME + ".tar.xz -P /mnt/gentoo");
                executeCommand("cd /mnt/gentoo && tar xpvf stage3-arm64-systemd-" + DATETIME + ".tar.xz --xattrs-include='*.*' --numeric-owner");
                break;
            case 2:
                executeCommand("wget https://distfiles.gentoo.org/releases/arm64/autobuilds/" + DATETIME + "/stage3-arm64-desktop-openrc-" + DATETIME + ".tar.xz -P /mnt/gentoo");
                executeCommand("cd /mnt/gentoo && tar xpvf stage3-arm64-desktop-openrc-" + DATETIME + ".tar.xz --xattrs-include='*.*' --numeric-owner");
                break;
            case 3:
                executeCommand("wget https://distfiles.gentoo.org/releases/arm64/autobuilds/" + DATETIME + "/stage3-arm64-desktop-systemd-" + DATETIME + ".tar.xz -P /mnt/gentoo");
                executeCommand("cd /mnt/gentoo && tar xpvf stage3-arm64-desktop-systemd-" + DATETIME + ".tar.xz --xattrs-include='*.*' --numeric-owner");
                break;
        }
    }
}
