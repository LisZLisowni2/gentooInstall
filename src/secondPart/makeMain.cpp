#include "InstallerSecond.hpp"
#include "utils.hpp"
#include <string>
#include <iostream>
#include <stdexcept>
#include <map>
#include <vector>

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/app.hpp>
#include <ftxui/component/event.hpp>

using namespace ftxui;

void InstallerSecond::makeMain() {
    std::vector<std::string> options = {
        "Wizard - not supported currently",
        "Edit file manually",
        "Continue",
    };

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

    while (true) {
        screen.Loop(inputHandler);

        if (options[selected] != "Continue") {
            switch (selected) {
                case 0:
                    break;
                case 1:
                    executeCommand("nano /etc/portage/make.conf");
                    break;
            }
        } else {
            break;
        }
    }
}
