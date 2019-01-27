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
else ()
    set(BASE_OUT_DIR out-x86)
endif ()

# Output Directory Setup
file(MAKE_DIRECTORY "./${BASE_OUT_DIR}")
file(MAKE_DIRECTORY "./${BASE_OUT_DIR}/lib")    # Library files (.a, .so, etc.)
file(MAKE_DIRECTORY "./${BASE_OUT_DIR}/bin")    # Binary files (piboot, etc.)
file(MAKE_DIRECTORY "./${BASE_OUT_DIR}/img")    # Kernel image files (bootloader/kernel.img, etc.)

# Resolve absolute pathes to these directories
get_filename_component(LIB_OUT_DIR ${BASE_OUT_DIR}/lib REALPATH)
get_filename_component(EXE_OUT_DIR ${BASE_OUT_DIR}/bin REALPATH)
get_filename_component(IMG_OUT_DIR ${BASE_OUT_DIR}/img REALPATH)

set(CMAKE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/cmake)

########################################
#> End Project Setup                  <#
########################################

########################################
# Default Option Definitions           #
########################################

# ARM options
if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ARM")
    set(LINKER_SCRIPT linker/script-arm)
    set(LINK_OPTIONS -nostdlib)
    set(COMPILE_OPTIONS -nostdlib -nostartfiles -ffreestanding -fno-builtin)

# X86 Options
else ()
    set(LINKER_SCRIPT linker/script-x86)
    set(LINK_OPTIONS)
    set(COMPILE_OPTIONS -Og -ggdb -Wall)
endif ()

########################################
#> End Default Option Definitions     <#
########################################

########################################
# Submodule Include                    #
########################################

# Libraries
add_subdirectory(lib/startx86)
add_subdirectory(lib/libdebug)
add_subdirectory(lib/libtypes)
add_subdirectory(lib/libmemory)
add_subdirectory(lib/libboot)
add_subdirectory(lib/libpi)

# Pi Modules
add_subdirectory(modules/bootloader)
add_subdirectory(modules/blink)

# Pi Bootloader Client
add_subdirectory(piboot)

########################################
#> End Submodule Setup                <#
########################################

########################################
# Metatarget Setup                     #
########################################

# Target to build all targets; added to default for GNU make.
add_custom_target(build_all ALL DEPENDS libpi bootloader blink)

# Also make the bootloader client if we're compiling for x86
if (NOT ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ARM")
    add_dependencies(build_all piboot)
endif ()


########################################
#> End Metatarget Setup               <#
########################################