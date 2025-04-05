#include "InstallerFirst.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <map>

void InstallerFirst::chroot() {
    std::cout << "Now you will chroot to system. After script execution end second part of installer will run";
    getch();
    clearScreen();
    executeCommand("cp --dereference /etc/resolv.conf /mnt/gentoo/etc");
    executeCommand("mount --types proc /proc /mnt/gentoo/proc && mount --rbind /sys /mnt/gentoo/sys && mount --make-rslave /mnt/gentoo/sys &&mount --rbind /dev /mnt/gentoo/dev && mount --make-rslave /mnt/gentoo/dev && mount --bind /run /mnt/gentoo/run && mount --make-slave /mnt/gentoo/run");
    executeCommand("mkdir -p /mnt/gentoo/tmp/build && mv ./* /mnt/gentoo/tmp/build/");
    executeCommand("chroot /mnt/gentoo/");
}
