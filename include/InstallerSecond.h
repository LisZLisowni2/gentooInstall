#ifndef INSTALLER_SECOND_H
#define INSTALLER_SECOND_H

#include <string>
#include "GentooInstaller.h"

class InstallerSecond : GentooInstaller {
public:
    InstallerSecond();
    ~InstallerSecond();
    void install();
private:
    void startMenu();
};

#endif