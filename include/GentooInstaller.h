#ifndef GENTOO_INSTALLER_H
#define GENTOO_INSTALLER_H
#pragma once

#include <string>

class GentooInstaller {
public:
    virtual ~GentooInstaller() = default;
    virtual void install() = 0;
};

#endif // GENTOO_INSTALLER_H