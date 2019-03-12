##########
# CS140E: PiOS
# Jonathan Kula
# Winter 2019
#
# This build file is basically a meta-build-file that orchestrates
# and unifies the build files for each submodule, providing resolution
# for all of them.
#
# This build file was last updated on January 27th, 2019.
# If you're unfamiliar with CMake, you can just use the
# build.sh file present in the project root directory.
#
# This project should be generated with an ARM toolchain;
# please build with -DCMAKE_TOOLCHAIN_FILE=/path/to/arm-gcc-toolchain.cmake

cmake_minimum_required(VERSION 3.10)

########################################
# Project Setup                        #
########################################

project(PiOS C ASM)

# Output Base Directory
if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ARM")
    set(BASE_OUT_DIR out-arm)
    file(MAKE_DIRECTORY "./${BASE_OUT_DIR}/img")    # Kernel image files (bootloader/kernel.img, etc.)
    get_filename_component(IMG_OUT_DIR ${BASE_OUT_DIR}/img REALPATH)
else ()
    set(BASE_OUT_DIR out-x86)
    file(MAKE_DIRECTORY "./${BASE_OUT_DIR}/test")   # Test executables
    get_filename_component(TEST_OUT_DIR ${BASE_OUT_DIR}/test REALPATH)
endif ()

# Output Directory Setup
file(MAKE_DIRECTORY "./${BASE_OUT_DIR}")
file(MAKE_DIRECTORY "./${BASE_OUT_DIR}/lib")    # Library files (.a, .so, etc.)
file(MAKE_DIRECTORY "./${BASE_OUT_DIR}/bin")    # Binary files (piboot, etc.)

# Resolve absolute pathes to these directories
get_filename_component(LIB_OUT_DIR ${BASE_OUT_DIR}/lib REALPATH)
get_filename_component(EXE_OUT_DIR ${BASE_OUT_DIR}/bin REALPATH)

set(CMAKE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/cmake)

########################################
#> End Project Setup                  <#
########################################

include(${CMAKE_INCLUDE_DIR}/macros.cmake)

########################################
# Default Option Definitions           #
########################################

# ARM options
if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ARM")
    set(LINKER_SCRIPT linker/script-arm)
    set(LINK_OPTIONS -nostdlib)
    set(COMPILE_OPTIONS -nostdlib -nostartfiles -ffreestanding -fno-builtin -Wno-int-conversion)

    # X86 Options
else ()
    set(LINKER_SCRIPT linker/script-x86)
    set(LINK_OPTIONS)
    set(COMPILE_OPTIONS -Og -ggdb -Wall -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast)
endif ()

########################################
#> End Default Option Definitions     <#
########################################

########################################
# Submodule Include                    #
########################################

# Libraries
add_subdirectory(lib/libdebug)
add_subdirectory(lib/libtypes)
add_subdirectory(lib/libboot)
add_subdirectory(lib/libpi)
add_subdirectory(lib/libtest)
add_subdirectory(lib/libpreprocessor)
add_subdirectory(lib/libtty)
add_subdirectory(lib/libstart)
add_subdirectory(lib/libQ)
add_subdirectory(lib/libxbee)
add_subdirectory(lib/libzigbee)
add_subdirectory(lib/libsoftuart)

# Pi Modules
add_subdirectory(modules/bootloader)
add_subdirectory(modules/blink)
add_subdirectory(modules/vmm)
add_subdirectory(modules/gprof)
add_subdirectory(modules/sonar)
add_subdirectory(modules/thread-test)
add_subdirectory(modules/pishell-server)
add_subdirectory(modules/hello)
add_subdirectory(modules/xbee_echo)
add_subdirectory(modules/softuart-hello)
add_subdirectory(modules/softuart-echo)

# Unix Programs
add_subdirectory(programs/piboot)
add_subdirectory(programs/handoff)
add_subdirectory(programs/replay)
add_subdirectory(programs/echo)
add_subdirectory(programs/pishell)
add_subdirectory(programs/pifs)
add_subdirectory(programs/cmd-watch)

# Tests
add_subdirectory(test/memory-test)
add_subdirectory(test/blink-test)

########################################
#> End Submodule Setup                <#
########################################