#include "InstallerFirst.hpp"
#include "utils.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/app.hpp>
#include <ftxui/component/event.hpp>

using namespace ftxui;

std::string InstallerFirst::interfaceSelection() {
    executeCommand("(ip link | awk '{ print $2 }' | sed 's/:/*/') > /tmp/interfaces.tmp");
    std::ifstream interfacesFile("/tmp/interfaces.tmp");
    std::string line;
    std::vector<std::string> options = {};
    int index = 0;
    while (getline(interfacesFile, line)) {
        if (!line.empty() && line[line.length() - 1] == '\n') {
            line.erase(line.length() - 1);
        }

        if (line.at(line.length() - 1) == '*') { 
            line.erase(line.length() - 1);
            options.push_back(line);
            index++;
        }
    }

    int selected_index = 0;

    auto menu = Menu(&options, &selected_index); 

    auto layout = Renderer(menu, [&] {
        return vbox({
            text(" GentooInstall ") | bold | center | border,
            separator(),
            text("Use UP/DOWN arrow keys to navigate. Press ENTER to select"),
            separator(),
            menu->Render() | border,
        }) | border;
    });

    auto screen = App::Fullscreen();
    
    auto inputHandler = CatchEvent(layout, [&](Event event) {
        if (event == Event::Return) {
            screen.ExitLoopClosure()();
            return true;
        }

        return false;
    });

    screen.Loop(inputHandler);
    
    return options[selected_index];

    // clearScreen();
    // int key = selectMenu(options, "List of available network interfaces", "To configure Wifi choose correct internet interface, most often wireless interface starts with wl");
    // std::cout << "\n";
    
    // return options[key].title;
}

std::string InstallerFirst::wifiSelection() {
    std::vector<std::string> options = {};
    int selected_index = 0;

    while (true) {
        options.clear();
        executeCommand("(iwlist wlan0 scan | awk -F ':' '/ESSID:/ {gsub(/\"/,\"\",$2); if(!seen[$2]++) print $2}' | sed 's/\"//; s/\(.*\)\"$/\1/') > /tmp/wifi.tmp");
        std::ifstream interfacesFile("/tmp/wifi.tmp");
        std::string line;
        options.push_back("Refresh"); 
        
        int index = 0;
        while (getline(interfacesFile, line)) {
            if (!line.empty() && line[line.length() - 1] == '\n') {
                line.erase(line.length() - 1);
            }

            options.push_back(line);
            index++;
        }

        auto menu = Menu(&options, &selected_index); 

        auto layout = Renderer(menu, [&] {
            return vbox({
                text(" GentooInstall ") | bold | center | border,
                separator(),
                text("Use UP/DOWN arrow keys to navigate. Press ENTER to select"),
                separator(),
                menu->Render() | border,
            }) | border;
        });

        auto screen = App::Fullscreen();
        
        auto inputHandler = CatchEvent(layout, [&](Event event) {
            if (event == Event::Return) {
                screen.ExitLoopClosure()();
                return true;
            }

            return false;
        });

        screen.Loop(inputHandler);

        if (selected_index != 0) break;
    }
    
    return options[selected_index];
}

std::string InstallerFirst::passwordInput() {
    InputOption option;
    option.password = true;

    std::string password = "";
    std::string placeholder = "Type password for your wifi...";

    auto inputField = Input(&password, &placeholder, option);

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

        if (event == Event::Backspace && password.empty()) {
            return true;
        }

        return false;
    });

    screen.Loop(inputHandler);

    return password;
}

void InstallerFirst::networkConfig() {
    std::vector<std::string> options = {
        "Test the internet",
        "Configure Wifi",
        "Next"
    };

    int selected_index = 0;

    auto menu = Menu(&options, &selected_index); 

    auto layout = Renderer(menu, [&] {
        return vbox({
            text(" GentooInstall ") | bold | center | border,
            separator(),
            text("Use UP/DOWN arrow keys to navigate. Press ENTER to select"),
            separator(),
            menu->Render() | border,
        }) | border;
    });

    auto screen = App::Fullscreen();
    
    auto inputHandler = CatchEvent(layout, [&](Event event) {
        if (event == Event::Return) {
            screen.ExitLoopClosure()();
            return true;
        }

        return false;
    });

    while (true) {
        screen.Loop(inputHandler);
    
        switch (selected_index) {
            case 0:
                executeCommand("ping google.com -c 5");
                break;
            case 1: {
                std::string interface;
                std::string ssid;
                std::string password;
                interface = interfaceSelection();
                ssid = wifiSelection();
                password = passwordInput();
                break; 
            }
            case 2:
                return;
        };
    }
}