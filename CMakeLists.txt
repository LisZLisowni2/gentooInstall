cmake_minimum_required(VERSION 3.10)
project(gentooInstall)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

include_directories(include)
find_package(termcolor REQUIRED)
target_link_libraries(gentooInstall termcolor::termcolor)

set(SOURCES 
    src/main.cpp
    src/InstallerFirst.cpp
    src/InstallerSecond.cpp
    src/utils.cpp
    src/startMenu.cpp
)

add_executable(installer_part1 
    src/InstallerFirst.cpp
    src/main.cpp
    src/startMenu.cpp
    utils/utils.cpp
)

add_executable(installer_part2
    src/InstallerSecond.cpp
    utils/utils.cpp
)