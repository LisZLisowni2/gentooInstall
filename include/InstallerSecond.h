#ifndef INSTALLER_SECOND_H
#define INSTALLER_SECOND_H
#pragma once

#include <string>
#include "GentooInstaller.h"

class InstallerSecond : GentooInstaller {
public:
    InstallerSecond();
    ~InstallerSecond();
    void install() override;
private:
    bool isOpenRC;
    inline void makeMain();
    inline void profile();
    inline void kernelConfig();
    inline void kernelInstall();
    inline void kernelCompile();
    inline void networkConfig();
    inline void bootloader();
    inline void isValidUsername(const std::string& username);
    inline void userExists(const std::string& username);
    inline void userCreation();
    inline int profileSelection();
};

class UsernameInvalid : public std::exception {
    private:
        char* message;
    public:
        UsernameInvalid(char* msg) : message(msg) {}
        char* what() {
            return message;
        }
};

#endif