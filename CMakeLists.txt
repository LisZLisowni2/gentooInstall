cmake_minimum_required(VERSION 3.10)
project(gentooInstall)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

include_directories(include)
add_subdirectory(termcolor)
include_directories(${termcolor_SOURCE_DIR}/include)

set(SOURCES 
    src/main.cpp
    src/InstallerFirst.cpp
    src/InstallerSecond.cpp
    src/networkConfig.cpp
    utils/utils.cpp
    src/startMenu.cpp
    src/disk.cpp
    src/format.cpp
    src/mount.cpp
    utils/Option.cpp
)

add_executable(installer_part1 
    src/InstallerFirst.cpp
    src/main_part1.cpp
    src/startMenu.cpp
    src/networkConfig.cpp
    utils/utils.cpp
    src/disk.cpp
    src/format.cpp
    src/mount.cpp
    utils/Option.cpp
)

add_executable(installer_part2
    src/InstallerSecond.cpp
    utils/utils.cpp
    src/main_part2.cpp
    utils/Option.cpp
)