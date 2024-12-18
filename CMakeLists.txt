cmake_minimum_required(VERSION 3.10)
project(gentooInstall)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

include_directories(include)

set(SOURCES 
    src/main.cpp
    src/InstallerFirst.cpp
    src/InstallerSecond.cpp
    src/utils.cpp
)

add_executable(installer_part1 
    src/InstallerFirst.cpp
    src/main.cpp
    utils/utils.cpp
)

add_executable(installer_part2
    src/InstallerSecond.cpp
    utils/utils.cpp
)