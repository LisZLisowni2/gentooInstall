cmake_minimum_required(VERSION 3.10)
project(gentooInstall)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_INSTALL_RPATH "\$ORIGIN/_deps/cpr-build/cpr:\$ORIGIN/_deps/curl-build/lib:\$ORIGIN/_deps/libssh2-build/src:\$ORIGIN/_deps/zlib-build:\$ORIGIN/_deps")
set(CMAKE_BUILD_WITH_INSTALL_RPATH true)
set(ENABLE_STATIC ON CACHE BOOL "Enable static library" FORCE)
# set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
# set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

option(FIRST_INSTALLER_INCLUDE "First installer has enabled, compiling first part." ON)

include(FetchContent)

set(CPR_BUILD_SHARED_LIBS OFF CACHE BOOL "Use static cpr instead of shared" FORCE)
FetchContent_Declare(
    cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG dd967cb48ea6bcbad9f1da5ada0db8ac0d532c06
    OVERRIDE_FIND_PACKAGE    
)
FetchContent_MakeAvailable(cpr)
include_directories(${PROJECT_SOURCE_DIR}/build/_deps/cpr-src/include) 
include_directories(${PROJECT_SOURCE_DIR}/build/_deps/cpr-build/cpr_generated_includes)

set(LIBSSH2_BUILD_EXAMPLES OFF CACHE BOOL "Disable libssh2 examples" FORCE)
set(LIBSSH2_BUILD_TESTING OFF CACHE BOOL "Disable libssh2 testing" FORCE)
set(LIBSSH2_BUILD_SHARED_LIBS OFF CACHE BOOL "Use static libssh2 instead of shared" FORCE)
set(LIBSSH2_USE_STATIC_LIBS ON CACHE BOOL "Use static libssh2" FORCE)
set(PC_LIBSSH2_STATIC_LDFLAGS "-L/usr/lib -lssh2 -lssl -lcrypto -ldl -pthread -lz")
set(PC_LIBSSH2_STATIC_LIBRARIES "ssh2;ssl;crypto;dl;z")

FetchContent_Declare(
    libssh2
    GIT_REPOSITORY https://github.com/libssh2/libssh2.git
    GIT_TAG a312b43325e3383c865a87bb1d26cb52e3292641
)
FetchContent_MakeAvailable(libssh2)

add_library(libssh2_static STATIC IMPORTED)
set_target_properties(libssh2_static PROPERTIES
    LINK_SEARCH_START_STATIC 1
    LINK_SEARCH_END_STATIC 1
    IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/build/_deps/libssh2-build/src/libssh2.a"
    INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/build/_deps/libssh2-src/include"
)


find_library(LIBSSH2_LIBRARY NAMES libssh2.a PATHS ${CMAKE_BINARY_DIR}/_deps/libssh2-build/src)
message(STATUS "Using libssh2: ${LIBSSH2_LIBRARY}")
set(LIBSSH2_ROOT ${CMAKE_BINARY_DIR}/_deps/libssh2-build)
set(LIBSSH2_LIBRARY ${LIBSSH2_ROOT}/src/libssh2.a)
set(LIBSSH2_INCLUDE_DIR ${CMAKE_BINARY_DIR}/_deps/libssh2-src/include)
message(STATUS "Forcing libssh2: ${LIBSSH2_LIBRARY}")
include_directories(${LIBSSH2_INCLUDE_DIR})

include_directories(include)
add_subdirectory(termcolor)
include_directories(${termcolor_SOURCE_DIR}/include)

set(SOURCES 
    src/firstPart/main.cpp
    src/secondPart/main.cpp
    src/firstPart/InstallerFirst.cpp
    src/secondPart/InstallerSecond.cpp
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
add_library(${PROJECT_NAME} STATIC ${SOURCES})

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
    add_dependencies(installer_part1 cpr libssh2_static)
    target_link_libraries(installer_part1 PRIVATE cpr::cpr ${LIBSSH2_LIBRARY})
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
add_dependencies(installer_part2 cpr libssh2_static)
target_link_libraries(installer_part2 PRIVATE cpr::cpr ${LIBSSH2_LIBRARY})