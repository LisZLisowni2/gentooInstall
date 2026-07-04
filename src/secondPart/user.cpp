#include "InstallerSecond.hpp"
#include "utils.hpp"
#include <string>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <map>
#include <vector>
#include <regex>
#include <fstream>

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/app.hpp>

using namespace ftxui;

void InstallerSecond::isValidUsername(const std::string& username) {
    // Check if username is empty or too long
    if (username.empty() || username.length() > 32) {
        throw UsernameInvalid("Username length isn't correct (must be 1-32 characters)");
    }
    
    // Username should start with a letter or underscore
    if (!std::isalpha(username[0]) && username[0] != '_') {
        throw UsernameInvalid("Username doesn't start with a letter or underscore");
    }
    
    // Username can only contain letters, numbers, underscores, and hyphens
    std::regex usernameRegex("^[a-zA-Z_][a-zA-Z0-9_-]*$");
    if (!std::regex_match(username, usernameRegex)) {
        throw UsernameInvalid("Username contains forbidden characters");
    }
    
    // Check for reserved usernames
    std::vector<std::string> reservedNames = {
        "root", "bin", "daemon", "sys", "adm", "tty", "disk", "lp", 
        "mail", "news", "uucp", "man", "proxy", "kmem", "dialout", 
        "fax", "voice", "cdrom", "floppy", "tape", "sudo", "audio", 
        "dip", "www-data", "backup", "operator", "list", "irc", 
        "src", "gnats", "shadow", "utmp", "video", "sasl", "plugdev", 
        "staff", "games", "users", "nogroup", "nobody", "systemd-journal",
        "systemd-network", "systemd-resolve", "wheel", "portage"
    };
    
    for (const auto& reserved : reservedNames) {
        if (username == reserved) {
            throw UsernameInvalid("This username is reserved and is forbidden");
        }
    }
}

void InstallerSecond::userExists(const std::string& username) {
    std::ifstream passwdFile("/etc/passwd");
    if (!passwdFile.is_open()) {
        throw UsernameInvalid("Cannot access user database to verify username");
    }
    std::string line;
    
    while (getline(passwdFile, line)) {
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {
            std::string existingUser = line.substr(0, colonPos);
            if (existingUser == username) {
                throw UsernameInvalid("Username is already taken");
            }
        }
    }
}

void InstallerSecond::userCreation() {
    std::string username;
    std::string placeholder = "Enter username...";
    std::string error = "";
    
    auto screen = App::Fullscreen();
    auto inputField = Input(&username, &placeholder);

    auto layout = Renderer(inputField, [&] {
        return vbox({
            text(" GentooInstall "),
            separator(),
            window(text(""), inputField->Render()),
            separator(),
            vbox({
                text("Username requirements:"),
                text("- Must start with a letter or underscore"),
                text("- Can contain letters, numbers, underscores, and hyphens"),
                text("- Must be between 1-32 characters\n"),
                text("- Cannot be a system reserved name\n\n"),
                separator(),
                text("Status: " + error),
            })
        });
    });

    auto inputHandler = CatchEvent(layout, [&](Event event) {
        if (event == Event::Return) {
            try {
                username.erase(0, username.find_first_not_of(" \t\n\r\f\v"));
                username.erase(username.find_last_not_of(" \t\n\r\f\v") + 1);

                isValidUsername(username);
                userExists(username);
                
                screen.ExitLoopClosure()();
            } catch (const UsernameInvalid& err) {
                error = err.what();
            }
            return true;
        }

        if (event == Event::Backspace && username.empty()) {
            return true;
        }

        return false;
    });

    screen.Loop(inputHandler);

    executeCommand("useradd -mG wheel,audio,video,usb,cdrom -s /bin/bash " + username);

    std::string password = "";
    placeholder = "Input password...";
    InputOption passwordInputOption;
    passwordInputOption.password = true;

    auto passwordInput = Input(&password, &placeholder, passwordInputOption);

    layout = Renderer(passwordInput, [&] {
        return vbox({
            text(" GentooInstall "),
            separator(),
            window(text(""), passwordInput->Render()),
        });
    });

    inputHandler = CatchEvent(layout, [&](Event event) {
        if (event == Event::Return) {
            try {
                executeCommand("echo -e \"" + password + "\n" + password + "\" | passwd " + username);
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