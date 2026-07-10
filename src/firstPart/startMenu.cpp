#include "InstallerFirst.hpp"
#include "utils.hpp"
#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "ftxui/component/app.hpp" 
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
using namespace ftxui;

void InstallerFirst::startMenu() {
    auto screen = App::Fullscreen();

    bool isContinue = false;

    auto layout = Renderer([&] {
        return vbox({
            text(" Welcome To GentooInstall ") | bold | center | border,
            text(" Press 'Enter' to continue ") | bold | center,
            text(" Press 'q' to exit ") | bold | center,
        }) | border;
    });

    auto inputHandler = CatchEvent(layout, [&](Event event) {
        if (event == Event::Return) {
            isContinue = true;
            screen.ExitLoopClosure()();
            return true;
        } else if (event == Event::q) {
            screen.ExitLoopClosure()();
            return true;
        }

        return false;
    });

    screen.Loop(inputHandler);

    if (!isContinue) {
        exit(0);
    }
}