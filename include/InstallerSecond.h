#ifndef INSTALLER_FIRST_H
#define INSTALLER_FIRST_H

#include <string>
#include "GentooInstaller.h"

class InstallerFirst : GentooInstaller {
public:
    InstallerFirst();
    ~InstallerFirst();
    void install();
private:
    void startMenu();
};

#endif