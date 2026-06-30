#include "InstallerFirst.hpp"
#include "utils.hpp"
#include <string>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <map>

void InstallerFirst::chroot() {
    clearScreen();
    executeCommand("sudo cp --dereference /etc/resolv.conf /mnt/gentoo/etc");
    executeCommand("sudo mount --types proc /proc /mnt/gentoo/proc && sudo mount --rbind /sys /mnt/gentoo/sys && sudo mount --make-rslave /mnt/gentoo/sys && sudo mount --rbind /dev /mnt/gentoo/dev && sudo mount --make-rslave /mnt/gentoo/dev && sudo mount --bind /run /mnt/gentoo/run && sudo mount --make-slave /mnt/gentoo/run");
    executeCommand("mkdir -p /mnt/gentoo/tmp/build && mv ./* /mnt/gentoo/tmp/build/");
    std::string chrootCommand = 
        "sudo chroot /mnt/gentoo/ bash -c '"
        "source /etc/profile && "
        "export PS1=\"(chroot) ${PS1}\" &&"
        "cd /tmp/build/ && "
        "./installer_part2"
        "'";
    executeCommand(chrootCommand);
}
