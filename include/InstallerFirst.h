#ifndef INSTALLER_FIRST_H
#define INSTALLER_FIRST_H
#pragma once

#include <string>
#include "GentooInstaller.h"

class InstallerFirst : GentooInstaller {
public:
    InstallerFirst();
    ~InstallerFirst();
    void install() override;
private:
    inline void startMenu();
};

#endif