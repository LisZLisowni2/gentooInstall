#ifndef INSTALLER_FIRST_HPP
#define INSTALLER_FIRST_HPP
#pragma once

#include <string>
#include "GentooInstaller.hpp"

class InstallerFirst : GentooInstaller {
public:
    InstallerFirst();
    ~InstallerFirst();
    void install() override;
private:
    inline void startMenu();
    inline void networkConfig();
    inline void partitions();
    inline void format();
    inline void mount();
    inline void tarball();
    inline void chroot();
    inline std::string interfaceSelection();
    inline std::string wifiSelection();
    inline std::string partitionSelection();
    inline std::string formatSelection();
};

#endif