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

void InstallerSecond::zoneConfig() {
    executeCommand("chmod +x /build/scripts/zone.sh");
    executeCommand("chmod +x /build/scripts/subZone.sh");
    executeCommand("/build/scripts/zone.sh");
    std::ifstream zoneFile("/tmp/zone.tmp");
    std::string line;
    std::vector<std::string> options = {};
    while (getline(zoneFile, line)) {
        if (!line.empty() && line[line.length() - 1] == '\n') {
            line.erase(line.length() - 1);
        }

        options.push_back(line);
    }

    zoneFile.close();

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

    std::string rootZone = options[selected];

    // executeCommand("rm /tmp/zone.tmp");
    executeCommand("/build/scripts/subZone.sh " + rootZone);
    std::ifstream subZoneFile("/tmp/subZone.tmp");
    options.clear();
    while (getline(subZoneFile, line)) {
        if (!line.empty() && line[line.length() - 1] == '\n') {
            line.erase(line.length() - 1);
        }

        options.push_back(line);
    }

    subZoneFile.close();

    selected = 0;
    screen.Loop(inputHandler);

    // clearScreen();
    // key = selectMenu<std::string>(options, "List of available subzones", "Choose correct subzone");
    // std::cout << "\n";
    // executeCommand("rm /tmp/zone.tmp");
    // executeCommand("rm /tmp/subZone.sh");
    // executeCommand("rm /tmp/zone.sh");
    executeCommand("ln -sf /usr/share/zoneinfo/" + rootZone + "/" + options[selected] + " /etc/localtime");
}