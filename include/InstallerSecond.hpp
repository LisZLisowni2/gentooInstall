#ifndef INSTALLER_SECOND_HPP
#define INSTALLER_SECOND_HPP
#pragma once

#include <string>
#include "GentooInstaller.hpp"

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
    inline void zoneConfig();
    inline void bootloader();
    inline void utils();
    inline void isValidUsername(const std::string& username);
    inline void userExists(const std::string& username);
    inline void userCreation();
    inline std::string diskSelection();
    inline int profileSelection();
};

class UsernameInvalid : public std::exception {
    private:
        std::string message;
    public:
        UsernameInvalid(const std::string& msg) : message(msg) {}
        const char* what() const noexcept override {
            return message.c_str();
        }
};

#endif