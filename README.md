# GentooInstaller

## Introduction

...

## Installation

Installation of GentooInstaller depends on your needs. If you want install all your system using GentooInstaller use first or third method,<br>
if you want compile, use second method.

### First method: Precompiled

Download the precompiled files in bin folder.

`wget LINK_TO_BIN_RELEASE`

Move to created folder, run `./installer_part1` and follow the instructions.

### Second method: Compilation from source in chrooted environment

**WARNING! YOU HAVE TO CONFIG NETWORK, CONFIG DISK'S PARTITIONS, MOUNTING PARTITIONS, DOWNLOAD TARBALL FILE, COPY DNS INFO AND MOUNT NECESSARY PARTITIONS (LIKE /proc) AND CHROOTING TO NEW SYSTEM BY YOURSELF BEFORE USING THAT METHOD.**

Download all files from github.

`wget https://github.com/LisZLisowni2/GentooInstaller`

Move to created folder and compile the program using that command:

`mkdir build && cd build && cmake -DFIRST_INSTALLER_INCLUDE=OFF .. && make`

Run `./installer_part2` and follow the instructions.

### Third method: Compilation from source in host system

To use that method you have to compile it in your host system. Make sure you have git, cmake, make and gcc/g++ compilers.

Download all files from github.

`git clone https://github.com/LisZLisowni2/GentooInstaller`

Move to created folder and compile the program using that command:

`mkdir build && cd build && cmake .. && make`

Run `./installer_part1` and follow the instructions.