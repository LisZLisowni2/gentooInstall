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

void InstallerSecond::localeSelection() {
    executeCommand("(cat /etc/locale.gen | tail -n+3 | awk '{ print $2 }') > /tmp/localeValues.tmp");
    executeCommand("(cat /etc/locale.gen | tail -n+3 | awk \'{ s = \"\"; for (i = 4; i <= NF; i++) s = s \" \" $i; print s }\') > /tmp/localeNames.tmp");
    std::ifstream localeNameFile("/tmp/localeNames.tmp");
    std::ifstream localeValueFile("/tmp/localeValues.tmp");
    std::string line;
    std::vector<std::string> names = {};
    std::vector<std::string> values = {};
    while (getline(localeNameFile, line)) {
        if (!line.empty() && line[line.length() - 1] == '\n') {
            line.erase(line.length() - 1);
        }

        names.push_back(line);
    }

    while (getline(localeValueFile, line)) {
        if (!line.empty() && line[line.length() - 1] == '\n') {
            line.erase(line.length() - 1);
        }

        values.push_back(line);
    }
    
    int selected = 0;

    auto screen = App::Fullscreen();

    auto menu = Menu(&names, &selected);

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

    std::cout << "\n";
    if (values[selected].find('@') == std::string::npos) {
        executeCommand("echo \"" + values[selected] + ".UTF-8 UTF-8\" >> /etc/locale.gen");
    } else {
        std::string newString = values[selected].substr(0, values[selected].find("@") - 1) + ".UTF-8" + values[selected].substr(values[selected].find("@") - 1) + " UTF-8";
        executeCommand("echo \"" + newString + "\" > /etc/locale.gen");
    }
}

void InstallerSecond::localeConfig() {
    std::vector<std::string> locales = {};

    std::vector<std::string> options = {
        "Add locale",
        "Continue",
    };

    // while (true) {
    //     localeSelection();
    //     clearScreen();
    //     int key = selectMenu(options, "Do you want add more locales?", "Choose correct option");
    //     std::cout << "\n";
    //     if (key == -1) {
    //         break;
    //     }
    // }

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
        if (options[selected] == "Continue") {
            break;
        } else {
            localeSelection();
        }
    }

    executeCommand("locale-gen");
    executeCommand("eselect locale list | tail -n+1 | awk '{ print $2 }' > /tmp/locales.tmp");
    std::ifstream localeFile("/tmp/locales.tmp");
    std::string line;
    int value;
    options.clear();
    int index = 0;
    while (getline(localeFile, line)) {
        if (!line.empty() && line[line.length() - 1] == '\n') {
            line.erase(line.length() - 1);
        }

        options.push_back(line);
        index++;
    }

    selected = 0;

    screen.Loop(inputHandler);

    // clearScreen();
    // int key = selectMenu(options, "List of generated locales", "Choose correct locale");
    // std::cout << "\n";
    executeCommand("eselect locale set " + options[selected]);
}