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
    inline void makeMain();
    inline void profile();
    inline void kernelConfig();
    inline void kernelInstall();
    inline void networkConfig();
    inline void reboot();
};

#endif