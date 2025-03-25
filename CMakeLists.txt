cmake_minimum_required(VERSION 3.10)
project(gentooInstall)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_INSTALL_RPATH "\$ORIGIN/_deps")
set(CMAKE_BUILD_WITH_INSTALL_RPATH true)
set(BUILD_SHARED_LIBS OFF)

include(FetchContent)

FetchContent_Declare(
    cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG dd967cb48ea6bcbad9f1da5ada0db8ac0d532c06
    OVERRIDE_FIND_PACKAGE    
)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(
    libssh2
    GIT_REPOSITORY https://github.com/libssh2/libssh2.git
    GIT_TAG a312b43325e3383c865a87bb1d26cb52e3292641
)
FetchContent_MakeAvailable(libssh2)

add_library(libssh2_static STATIC IMPORTED)
set_target_properties(libssh2_static PROPERTIES
    IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/_deps/libssh2-build/src/libssh2.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_BINARY_DIR}/_deps/libssh2-src/include"
)

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
    Dev/main.cpp
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
    target_link_libraries(installer_part1 PRIVATE cpr libssh2_static)
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
target_link_libraries(installer_part2 PRIVATE cpr libssh2_static)