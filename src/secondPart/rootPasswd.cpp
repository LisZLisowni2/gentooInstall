#include "InstallerSecond.hpp"
#include "utils.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <map>
#include <vector>

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/app.hpp>
#include <ftxui/component/event.hpp>

using namespace ftxui;

void InstallerSecond::rootPasswd() {
    std::string password = "";
    std::string placeholder = "Input password for root...";
    InputOption passwordInputOption;
    passwordInputOption.password = true;

    auto screen = App::Fullscreen();

    auto passwordInput = Input(&password, &placeholder, passwordInputOption);

    auto layout = Renderer(passwordInput, [&] {
        return vbox({
            text(" GentooInstall "),
            separator(),
            window(text(""), passwordInput->Render()),
        });
    });

    auto inputHandler = CatchEvent(layout, [&](Event event) {
        if (event == Event::Return) {
            try {
                executeCommand("echo -e \"" + password + "\n" + password + "\" | passwd");
                screen.ExitLoopClosure()();
            } catch (...) {
                NULL;
            }
            return true;
        }

        if (event == Event::Backspace && password.empty()) {
            return true;
        }

        return false;
    });

    screen.Loop(inputHandler);
}