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

int InstallerSecond::profileSelection() { 
    executeCommand("(eselect profile list | grep \"default\" | sed 's/ //g; s/\\[//g; s/\\]/?/g') > /tmp/profiles.tmp");
    std::ifstream profilesFile("/tmp/profiles.tmp");
    std::string line;
    std::vector<std::string> options = {};

    while (getline(profilesFile, line)) {
        if (!line.empty() && line[line.length() - 1] == '\n') {
            line.erase(line.length() - 1);
        }

        std::string value = line.substr(line.find('?') + 1, line.length());
        options.push_back(value);
    }

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

    return selected + 1; // Profiles indexes starts with 1
}

void InstallerSecond::profile() {
    std::vector<std::string> options = {
        "Set profile",
        "Next",
    };
    
    auto screen = App::Fullscreen();

    int selected = 0;

    auto menu = Menu(&options, &selected);
    
    while (true) {
        std::string selectedProfile = "";
        executeCommand("eselect profile show | grep 'default' > /tmp/profile.tmp");
        std::ifstream file("/tmp/profile.tmp");
        
        getline(file, selectedProfile);

        auto layout = Renderer(menu, [&] {
            return vbox({
                text(" GentooInstall ") | bold | center | border,
                separator(),
                text("Use UP/DOWN arrow keys to navigate. Press ENTER to select"),
                separator(),
                menu->Render() | vscroll_indicator | frame | border | size(HEIGHT, LESS_THAN, 15),
                separator(),
                paragraph("Currently selected profile: " + selectedProfile) | bold,
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

        if (selected == 0) {
            int profile = profileSelection();

            executeCommand("eselect profile set " + std::to_string(profile));
        } else {
            break;
        }
    }
}
