#include "InstallerSecond.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <map>
#include <vector>
#include <regex>
#include <fstream>

void InstallerSecond::isValidUsername(const std::string& username) {
    // Check if username is empty or too long
    if (username.empty() || username.length() > 32) {
        throw UsernameInvalid("Username length isn't correct");
    }
    
    // Username should start with a letter or underscore
    if (!std::isalpha(username[0]) && username[0] != '_') {
        throw UsernameInvalid("Username doesn't start with a letter or underscore");
    }
    
    // Username can only contain letters, numbers, underscores, and hyphens
    std::regex usernameRegex("^[a-zA-Z_][a-zA-Z0-9_-]*$");
    if (!std::regex_match(username, usernameRegex)) {
        throw UsernameInvalid("Username contains forbidden chars");
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
    
    while (true) {
        clearScreen();
        std::cout << "User Account Creation\n";
        std::cout << "=====================\n\n";
        
        std::cout << "Username requirements:\n";
        std::cout << "- Must start with a letter or underscore\n";
        std::cout << "- Can contain letters, numbers, underscores, and hyphens\n";
        std::cout << "- Must be between 1-32 characters\n";
        std::cout << "- Cannot be a system reserved name\n\n";
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter your username: ";
        
        if (!getline(std::cin, username)) {
            std::cout << "Error reading input. Please try again.\n";
            std::cout << "Press any key to continue...";
            getch();
            continue;
        }
        
        // Trim whitespace
        username.erase(0, username.find_first_not_of(" \t\n\r\f\v"));
        username.erase(username.find_last_not_of(" \t\n\r\f\v") + 1);
        
        try {
            isValidUsername(username);
            userExists(username);
        } catch (UsernameInvalid err) {
            std::cout << err.what() << "\n";
            std::cout << "Press any key to continue...";
            getch();
            continue;
        }
        
        // Username is valid, break out of loop
        break;
    }
    
    std::cout << "\nCreating user '" << username << "'...\n";
    
    int result = executeCommand("useradd -mG wheel,audio,video,usb,cdrom -s /bin/bash " + username);
    
    if (result != 0) {
        std::cout << "\nError: Failed to create user '" << username << "'.\n";
        std::cout << "This might be due to system constraints or permissions.\n";
        std::cout << "Press any key to continue...";
        getch();
        return;
    }
    
    std::cout << "User '" << username << "' created successfully!\n\n";
    
    // Set password with validation
    while (true) {
        std::cout << "Setting password for user '" << username << "'...\n";
        int passwdResult = executeCommand("passwd " + username);
        
        if (passwdResult == 0) {
            std::cout << "\nPassword set successfully!\n";
            break;
        } else {
            std::cout << "\nPassword setting failed. This might be due to:\n";
            std::cout << "- Password doesn't meet system requirements\n";
            std::cout << "- Passwords don't match\n";
            std::cout << "- System error\n\n";
            
            std::cout << "Would you like to try again? (y/n): ";
            std::string retry;
            getline(std::cin, retry);
            
            if (retry.empty() || (retry[0] != 'y' && retry[0] != 'Y')) {
                std::cout << "Warning: User created but password not set!\n";
                std::cout << "You can set the password later using 'passwd " << username << "' command.\n";
                break;
            }
        }
    }
    
    std::cout << "\nUser creation completed.\n";
    std::cout << "Press any key to continue...";
    getch();
}