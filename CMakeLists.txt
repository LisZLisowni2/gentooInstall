cmake_minimum_required(VERSION 3.15)
project(gentooInstall)
include(GNUInstallDirs)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a;.lib")
set(ENABLE_STATIC ON CACHE BOOL "Enable static library" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Use static libraries" FORCE)
set(CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "Library install directory" FORCE)

option(FIRST_INSTALLER_INCLUDE "First installer has enabled, compiling first part." ON)

include(ExternalProject)
add_compile_definitions(__gnuc_va_list=va_list)

# OpenSSL
set(OPENSSL_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/openssl-src)
set(OPENSSL_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/openssl)
set(OPENSSL_INCLUDE_DIR ${CMAKE_INSTALL_DIR}/include)
set(OPENSSL_CONFIGURE_COMMAND ${OPENSSL_SOURCE_DIR}/config)
ExternalProject_Add(
    openssl
    SOURCE_DIR ${OPENSSL_SOURCE_DIR}
    GIT_REPOSITORY https://github.com/openssl/openssl.git
    GIT_TAG 636dfadc70ce26f2473870570bfd9ec352806b1d
    CMAKE_ARGS
        -DBUILD_SHARED_LIBS=OFF
        -DUSE_STATIC_LIBS=ON
        -DCMAKE_BUILD_TYPE=release
        # -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/_install
    CONFIGURE_COMMAND 
        ${OPENSSL_CONFIGURE_COMMAND}
            --prefix=${OPENSSL_INSTALL_DIR}
            --openssldir=${OPENSSL_INSTALL_DIR}
			--libdir=${OPENSSL_INSTALL_DIR}/lib
    BUILD_COMMAND make
    TEST_COMMAND ""
    INSTALL_COMMAND 
        make install && rm -rf ${CMAKE_CURRENT_BINARY_DIR}/openssl-*
    INSTALL_DIR ${OPENSSL_INSTALL_DIR}
)

add_library(OpenSSL_Lib STATIC IMPORTED GLOBAL)
add_library(OpenSSL::Crypto STATIC IMPORTED GLOBAL)

set_target_properties(OpenSSL_Lib PROPERTIES
    IMPORTED_LOCATION ${OPENSSL_INSTALL_DIR}/lib/libssl.a
)

set_target_properties(OpenSSL::Crypto PROPERTIES
    IMPORTED_LOCATION ${OPENSSL_INSTALL_DIR}/lib/libcrypto.a
)

if (EXISTS "${OPENSSL_INCLUDE_DIR}")
    set_target_properties(OpenSSL_Lib PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${OPENSSL_INCLUDE_DIR}
    )
    set_target_properties(OpenSSL::Crypto PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${OPENSSL_INCLUDE_DIR}
    )
endif()

add_dependencies(OpenSSL_Lib openssl)
add_dependencies(OpenSSL::Crypto STATIC IMPORTED GLOBAL)

# CPR
set(CPR_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/cpr-src)
set(CPR_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/cpr)
set(CPR_INCLUDE_DIR ${CPR_INSTALL_DIR}/include)
set(CPR_CONFIGURE_COMMAND cmake -B ${CPR_SOURCE_DIR}/build ${CPR_SOURCE_DIR})
ExternalProject_Add(
    cpr
    SOURCE_DIR ${CPR_SOURCE_DIR}
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG da40186618909b1a7363d4e4495aa899c6e0eb75
    CMAKE_ARGS
        -DBUILD_SHARED_LIBS=OFF
        -DUSE_STATIC_LIBS=ON
        -DCPR_ENABLE_SSL=ON
		-DWITH_SSL=ON
        -DWITH_LIBIDN2=ON
        -DWITH_LIBPSL=ON
		-DCURL_BROTLI=OFF
		-DCURL_ZSTD=OFF
		-DBUILD_TESTING=OFF
	 	-DCMAKE_INSTALL_PREFIX={CPR_INSTALL_DIR}
        -DCMAKE_INSTALL_LIBDIR={CPR_INSTALL_DIR}/lib
    CONFIGURE_COMMAND 
        ${CPR_CONFIGURE_COMMAND}
    BUILD_COMMAND make -C ${CPR_SOURCE_DIR}/build
    TEST_COMMAND ""
    INSTALL_COMMAND 
        make -C ${CPR_SOURCE_DIR}/build install && rm -rf ${CMAKE_CURRENT_BINARY_DIR}/cpr-*
    INSTALL_DIR ${CPR_INSTALL_DIR}
)

add_library(cpr_lib STATIC IMPORTED GLOBAL)
add_library(libcurl STATIC IMPORTED GLOBAL)
add_library(zlib_lib STATIC IMPORTED GLOBAL)

set_target_properties(cpr_lib PROPERTIES 
	IMPORTED_LOCATION "${CPR_INSTALL_DIR}/lib/libcpr.a"
)

set_target_properties(libcurl PROPERTIES
    IMPORTED_LOCATION "${CPR_INSTALL_DIR}/lib/libcurl.a"
)

set_target_properties(zlib_lib PROPERTIES
    IMPORTED_LOCATION "${CPR_INSTALL_DIR}/lib/libz.a"
)

if (EXISTS "${CPR_INCLUDE_DIR}")
    set_target_properties(cpr_lib PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${CPR_INCLUDE_DIR}
    )
    set_target_properties(zlib_lib PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${CPR_INCLUDE_DIR}
    )
    set_target_properties(libcurl PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${CPR_INCLUDE_DIR}
    )
endif()

add_dependencies(cpr_lib cpr)
add_dependencies(libcurl cpr)
##add_dependencies(zlib_lib cpr)


# Libssh2
##set(PC_LIBSSH2_STATIC_LDFLAGS "-L/usr/lib -lssh2 -lssl -lcrypto -ldl -pthread ")
##set(PC_LIBSSH2_STATIC_LIBRARIES "ssh2;ssl;crypto;dl;z")
set(LIBSSH2_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/libssh2-src)
set(LIBSSH2_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/libssh2)
set(LIBSSH2_INCLUDE_DIR ${LIBSSH2_INSTALL_DIR}/include)
set(LIBSSH2_CONFIGURE_COMMAND cmake -B ${LIBSSH2_SOURCE_DIR}/build ${LIBSSH2_SOURCE_DIR})
ExternalProject_Add(
    libssh2
    SOURCE_DIR ${LIBSSH2_SOURCE_DIR}
    GIT_REPOSITORY https://github.com/libssh2/libssh2.git
    GIT_TAG a312b43325e3383c865a87bb1d26cb52e3292641
    CMAKE_ARGS
        -DLIBSSH2_BUILD_EXAMPLES=OFF
        -DLIBSSH2_BUILD_TESTING=OFF
        -DBUILD_SHARED_LIBS=OFF
        -DUSE_STATIC_LIBS=ON
		-DCMAKE_INSTALL_PREFIX=${LIBSSH2_INSTALL_DIR}
  		-DCMAKE_INSTALL_LIBDIR=${LIBSSH2_INSTALL_DIR}/lib
    CONFIGURE_COMMAND 
        ${LIBSSH2_CONFIGURE_COMMAND}
    BUILD_COMMAND make -C ${LIBSSH2_SOURCE_DIR}/build
    TEST_COMMAND ""
    INSTALL_COMMAND 
        make -C ${LIBSSH2_SOURCE_DIR}/build install && rm -rf ${CMAKE_CURRENT_BINARY_DIR}/libssh2-*
    INSTALL_DIR ${LIBSSH2_INSTALL_DIR}
)

add_library(libssh2_lib STATIC IMPORTED GLOBAL)

set_target_properties(libssh2_lib PROPERTIES
    IMPORTED_LOCATION ${LIBSSH2_INSTALL_DIR}/lib/libssh2.a
)

if(EXISTS "${LIBSSH2_INCLUDE_DIR}")
    set_target_properties(libssh2_lib PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${LIBSSH2_INCLUDE_DIR}
    )
endif()

add_dependencies(libssh2_lib libssh2)

# Nghttp2
set(NGHTTP2_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/nghttp2-src)
set(NGHTTP2_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/nghttp2)
set(NGHTTP2_INCLUDE_DIR ${NGHTTP2_INSTALL_DIR}/include)
set(NGHTTP2_CONFIGURE_COMMAND cmake -B ${NGHTTP2_SOURCE_DIR}/build ${NGHTTP2_SOURCE_DIR})
ExternalProject_Add(
    nghttp2
    SOURCE_DIR ${NGHTTP2_SOURCE_DIR}
    GIT_REPOSITORY https://github.com/nghttp2/nghttp2.git
    GIT_TAG 319bf015de8fa38e21ac271ce2f7d61aa77d90cb
    CMAKE_ARGS
        -DENABLE_LIB_ONLY=ON
        -DBUILD_SHARED_LIBS=OFF
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        -DBUILD_STATIC_LIBS=ON
        -DUSE_STATIC_LIBS=ON
		-DCMAKE_INSTALL_LIBDIR=${NGHTTP2_INSTALL_DIR}/lib
        -DCMAKE_INSTALL_PREFIX=${NGHTTP2_INSTALL_DIR}
    CONFIGURE_COMMAND 
        ${NGHTTP2_CONFIGURE_COMMAND}         
    BUILD_COMMAND make -C ${NGHTTP2_SOURCE_DIR}/build
    TEST_COMMAND ""
    INSTALL_COMMAND 
        make -C ${NGHTTP2_SOURCE_DIR}/build install && rm -rf ${CMAKE_CURRENT_BINARY_DIR}/nghttp2-* 
    INSTALL_DIR ${NGHTTP2_INSTALL_DIR}
)

add_library(nghttp2_lib STATIC IMPORTED GLOBAL)

set_target_properties(nghttp2_lib PROPERTIES
    IMPORTED_LOCATION ${NGHTTP2_INSTALL_DIR}/lib/libnghttp2.a
)

if(EXISTS "${NGHTTP2_INCLUDE_DIR}")
    set_target_properties(nghttp2_lib PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${NGHTTP2_INCLUDE_DIR}
    )
endif()

add_dependencies(nghttp2_lib nghttp2)

include_directories(${OPENSSL_INCLUDE_DIR} ${CPR_INCLUDE_DIR} ${LIBSSH2_INCLUDE_DIR} ${NGHTTP2_INCLUDE_DIR})

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
    src/secondPart/bootloader.cpp
    utils/Option.cpp
    utils/requests.cpp
)
add_library("${PROJECT_NAME}_lib" STATIC ${SOURCES})
target_link_libraries("${PROJECT_NAME}_lib" PRIVATE cpr_lib libcurl OpenSSL_Lib OpenSSL::Crypto libssh2_lib zlib_lib nghttp2_lib idn2 psl ssl crypto dl pthread)

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
        utils/requests.cpp
    )
    target_link_libraries(installer_part1 PRIVATE "${PROJECT_NAME}_lib")
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
    src/secondPart/bootloader.cpp
)
