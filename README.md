# GentooInstaller

## Introduction

Gentoo linux is hard to install and only manual. You have to configure everything manually from language to kernel.
GentooInstaller is iteractive installation script for Gentoo that automate many processes.
It is easy to use using arrows and beginner-friendly.

## Features

- Automates the process of installation of Gentoo linux system
- Explanations and guides how to choose wisely
- Automates bootloader setup
- Automates configuration of the system

## Used libraries

- [**CPR**](https://github.com/libcpr/cpr) - Modern API to serve HTTP requests in C++
- [**termcolor**](https://github.com/ikalnytskyi/termcolor) - Single-header library to add colors in C++
- [**libssh2**](https://www.libssh2.org/) - Library to operate SSH2 protocol
- [**OpenSSL**](https://www.openssl.org/) - Software to provide secure communications over computer network
- [**nghttp2**](https://nghttp2.org/) - Implementaion of HTTP/2

## Installation

Installation of GentooInstaller depends on your needs. If you want install all your system using GentooInstaller use first or third method,<br>
if you want finish installation after chroot to environment, use second method.

### First method: Precompiled

Download the build directory in bin folder.

```
wget https://github.com/LisZLisowni2/gentooInstall/releases/download/2.0.0/gentooInstall
```

Move to created folder, run `./installer_part1` and follow the instructions.

### Second method: Compilation from source in chrooted environment (DO NOT USE IT RIGHT NOW! IN FUTURE WILL BE AVAILABLE)

**WARNING! YOU HAVE TO CONFIG NETWORK, CONFIG DISK'S PARTITIONS, MOUNTING PARTITIONS, DOWNLOAD TARBALL FILE, COPY DNS INFO AND MOUNT NECESSARY PARTITIONS (LIKE /proc) AND CHROOTING TO NEW SYSTEM BY YOURSELF BEFORE USING THAT METHOD.**

Sync with emerge using `emerge-webrsync`  

Install a CMake and git

```
emerge --ask cmake dev/vcs-git
```

Download all files from github and unpack it.

```
wget https://github.com/LisZLisowni2/gentooInstall/archive/refs/tags/v2.0.0.tar.gz
tar xzvf v2.0.0.tar.gz
cd gentooInstall-v2.0.0
git clone https://github.com/ikalnytskyi/termcolor.git
```

Move to created folder and compile the program using that command:

```
mkdir build
cd build
cmake -DFIRST_INSTALLER_INCLUDE=OFF ..
make
```


Run `./installer_part2` and follow the instructions.

### Third method: Compilation from source in host system

To use that method you have to compile it in your host system. Make sure you have cmake, make and gcc/g++ compilers.

Download all files from github.

```
wget https://github.com/LisZLisowni2/gentooInstall/archive/refs/tags/v2.0.0.tar.gz
tar xzvf v2.0.0.tar.gz
cd gentooInstall-v2.0.0
git clone https://github.com/ikalnytskyi/termcolor.git
```

Move to created folder and compile the program using that command:

```
mkdir build 
cd build
cmake ..
make
```

Run `./installer_part1` and follow the instructions.

## Support 

If you encounter the issue write it in issue section. Describe your machine, architecture, method you used to install the script, which step has issue, details of error and any other useful information.

Example:
Architecture: AMD64
Method of installation: First method
Step: Downloading tarball
Details: The tarball doesn't download correctly. I got information about no space left.
...HERE SOME LOGS FROM TERMINAL...

You can use bug or feature template also.

If you have idea how to improve programe, fork the repo, made changes you want and pull request to main repo.
