cmake_minimum_required(VERSION 3.10)
project(gentooInstall)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

option(FIRST_INSTALLER_INCLUDE "First installer has enabled, compiling first part." ON)

include_directories(include)
add_subdirectory(termcolor)
include_directories(${termcolor_SOURCE_DIR}/include)

set(SOURCES 
    src/firstPart/main.cpp
    src/secondPart/main.cpp
    src/firstPart/InstallerFirst.cpp
    src/firstSecond/InstallerSecond.cpp
    src/firstPart/networkConfig.cpp
    utils/utils.cpp
    src/firstPart/startMenu.cpp
    src/firstPart/disk.cpp
    src/firstPart/format.cpp
    src/firstPart/tarball.cpp
    src/firstPart/chroot.cpp
    src/firstPart/mount.cpp
    src/secondPart/makeMain.cpp
    src/secondPart/profile.cpp
    src/secondPart/kernel.cpp
    src/secondPart/networkConfig.cpp
    utils/Option.cpp
)

if (FIRST_INSTALLER_INCLUDE)
    message(STATUS "Installer first part included")
    add_executable(installer_part1 
        src/firstPart/InstallerFirst.cpp
        src/firstPart/main.cpp
        src/firstPart/startMenu.cpp
        src/firstPart/networkConfig.cpp
        utils/utils.cpp
        src/firstPart/disk.cpp
        src/firstPart/format.cpp
        src/firstPart/mount.cpp
        src/firstPart/tarball.cpp
        src/firstPart/chroot.cpp
        utils/Option.cpp
    )
else()
    message(STATUS "Installer first part excluded")
endif()

add_executable(installer_part2
    src/secondPart/InstallerSecond.cpp
    utils/utils.cpp
    src/secondPart/main.cpp
    utils/Option.cpp
    src/secondPart/makeMain.cpp
    src/secondPart/networkConfig.cpp
    src/secondPart/profile.cpp
    src/secondPart/kernel.cpp
)