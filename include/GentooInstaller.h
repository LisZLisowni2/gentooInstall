#ifndef GENTOO_INSTALLER_H
#define GENTOO_INSTALLER_H

#include <string>

class GentooInstaller {
public:
    virtual ~GentooInstaller() = default;
    virtual void install();
protected:
    GentooInstaller() = default;
};

#endif // GENTOO_INSTALLER_H