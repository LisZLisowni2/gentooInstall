#ifndef INSTALLER_SECOND_H
#define INSTALLER_SECOND_H
#pragma once

#include <string>
#include "GentooInstaller.h"k

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
    inline void passwordForAdmin();
    inline void additionalPrograms();
    inline void GRUB();
    inline void reboot();
};

#endif