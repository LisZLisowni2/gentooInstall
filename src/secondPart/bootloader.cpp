#include "InstallerSecond.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <map>
#include <vector>

void InstallerSecond::bootloader() {
    installPackages("sys-boot/grub");
    if (isEFI) executeCommand("grub-install --target=x86_64-efi --efi-directory=/boot/efi --bootloader-id=GENTOO");
    else executeCommand("grub-install --target=i386-pc");
    executeCommand("grub-mkconfig -o /boot/grub/grub.cfg");
    executeCommand("passwd");
}