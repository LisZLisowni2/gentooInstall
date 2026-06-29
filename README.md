![Static Badge](https://img.shields.io/badge/GPL-3?label=License)
![](https://img.shields.io/badge/Version-2.2-red?style=flat)

# GentooInstall 

### Introduction

Gentoo linux is hard to install and only manual. You have to configure everything manually from language to kernel.
GentooInstall is interactive installation script for Gentoo that automate many processes.
It is easy to use using arrows and beginner-friendly.

## Definition
GentooInstall is an interactive C++ CLI tool designed to automate the tedious parts of a Gentoo installation while keeping the flexibility that makes Gentoo great. No more copy-pasting from the handbook for hours—just follow the prompts and build your system.
All configuration is provided using official Handbook.

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
- [**FTXUI**](https://github.com/ArthurSonzogni/FTXUI) - C++ library for creating terminal user interfaces (TUI)

## Installation

Installation of GentooInstaller requires manual compilation on Gentoo LiveCD due to GLIBC version compatibility. 
**THE LIVEGUI USB IMAGE IS REQUIRED, NOT MINIMAL ONE**

1. Open terminal and download the latest version of GentooInstall (unzip it after download) or clone the repository
2. Move (by command `cd`) to that folder and run `cmake -S . -B build`
3. Move to `build` folder and run `make -j$(nproc) -l$(nproc)`
4. After compilation run installer: `./installer_part1`

## Support 

If you encounter the issue write it in issue section. Describe your machine, architecture, method you used to install the script, which step has issue, details of error and any other useful information.

Example:
Architecture: AMD64
Method of installation: First method
Step: Downloading tarball
Details: The tarball doesn't download correctly. I got information about no space left.
...HERE SOME LOGS FROM TERMINAL...

You can use bug or feature template also.

If you have idea how to improve program, fork the repo, made changes you want and pull request to main repo.
