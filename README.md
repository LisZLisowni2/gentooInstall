![Static Badge](https://img.shields.io/badge/GPL-3?label=License)
![](https://img.shields.io/badge/Version-3.0-red?style=flat)

# GentooInstall 

### Introduction

Gentoo linux is hard to install and only manual. You have to configure everything manually from language to kernel.
GentooInstall is interactive installation script for Gentoo that automate many processes.
It is easy to use using arrows and beginner-friendly.

## Definition
GentooInstall is an interactive C++ TUI tool designed to automate the tedious parts of a Gentoo installation while keeping the flexibility that makes Gentoo great. No more copy-pasting from the handbook for hours—just follow the prompts and build your system.
All configuration is provided using official Handbook.

## Features

- Automates the process of installation of Gentoo linux system
- Explanations and guides how to choose wisely
- Automates bootloader setup
- Automates configuration of the system

## Used libraries

- [**CPR**](https://github.com/libcpr/cpr) - Modern API to serve HTTP requests in C++
- [**libssh2**](https://www.libssh2.org/) - Library to operate SSH2 protocol
- [**OpenSSL**](https://www.openssl.org/) - Software to provide secure communications over computer network
- [**nghttp2**](https://nghttp2.org/) - Implementaion of HTTP/2
- [**FTXUI**](https://github.com/ArthurSonzogni/FTXUI) - C++ library for creating terminal user interfaces (TUI)

## Installation

> ⚠️ **IMPORTANT REQUIREMENT**  
> Due to `glibc` version dependencies and build requirements, **you MUST use the official Gentoo LiveGUI image**, not the Minimal Installation CD.

1. Boot into the **Gentoo LiveGUI** environment and open a terminal.
2. Clone this repository (or download and extract the latest release archive):
```bash
   git clone https://github.com/LisZLisowni2/gentooInstall.git
   cd gentooInstall
```
Run the automated build and launch script:
```bash
  chmod +x buildAndRun.sh
    ./buildAndRun.sh
```
## Contributing and Support 
Contributions, bug reports, and feature requests are very welcome!
### Reporting Issues

If you encounter a bug, please open an issue in the repository with the following details:
- Architecture: (e.g., amd64)
- Installation Stage: (e.g., Downloading tarball, Kernel configuration)
- Error Details: Relevant logs or error output from the terminal.

### Contributing
1. Fork the project repository.
2. Create your feature/bugfix branch (git checkout -b feature/AmazingFeature).
3. Commit your changes (git commit -m 'Add some AmazingFeature').
4. Push to the branch (git push origin feature/AmazingFeature).
5. Open a Pull Request against the main branch.
