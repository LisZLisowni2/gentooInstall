#include "InstallerFirst.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <map>

void InstallerFirst::chroot() {
    clearScreen();
    executeCommand("cp --dereference /etc/resolv.conf /mnt/gentoo/etc");
    executeCommand("mount --types proc /proc /mnt/gentoo/proc && mount --rbind /sys /mnt/gentoo/sys && mount --make-rslave /mnt/gentoo/sys &&mount --rbind /dev /mnt/gentoo/dev && mount --make-rslave /mnt/gentoo/dev && mount --bind /run /mnt/gentoo/run && mount --make-slave /mnt/gentoo/run");
    executeCommand("mkdir -p /mnt/gentoo/tmp/build && mv ./* /mnt/gentoo/tmp/build/");
    std::string chrootCommand = 
        "chroot /mnt/gentoo/ bash -c '"
        "source /etc/profile && "
        "export PS1=\"(chroot) ${PS1}\" &&"
        "cd /tmp/build/ && "
        "./installer_part2"
        "'";
    executeCommand(chrootCommand);
}
