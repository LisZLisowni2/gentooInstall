cmake_minimum_required(VERSION 3.10)
project(gentooInstall)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_INSTALL_RPATH "\$ORIGIN/libs:\$ORIGIN/_deps/cpr-build/cpr:\$ORIGIN/_deps/curl-build/lib:\$ORIGIN/_deps/zlib-build/:/usr/local/lib:/usr/lib64:/usr/lib")
set(CMAKE_BUILD_WITH_INSTALL_RPATH true)

include(FetchContent)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBSSH2 REQUIRED libssh2)

FetchContent_Declare(
    cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG dd967cb48ea6bcbad9f1da5ada0db8ac0d532c06
    OVERRIDE_FIND_PACKAGE    
)
set(CPR_BUILD_SHARED_LIBS OFF) 
FetchContent_MakeAvailable(cpr)

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
    target_link_libraries(installer_part1 PRIVATE cpr ssh2)
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
target_link_libraries(installer_part2 PRIVATE cpr ssh2)