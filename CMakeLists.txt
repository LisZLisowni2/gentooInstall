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

add_executable(gentoo_installer ${VERSION})