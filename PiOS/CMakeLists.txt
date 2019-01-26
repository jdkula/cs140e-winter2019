##########
# CS140E: PiOS
# Jonathan Kula
# Winter 2019
#
# This build file is basically a meta-build-file that orchestrates
# and unifies the build files for each submodule, providing resolution
# for all of them.
#
# This build file was last updated on January 25th, 2019.
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

# Output Directory Setup
file(MAKE_DIRECTORY "./out")
file(MAKE_DIRECTORY "./out/lib")    # Library files (.a, .so, etc.)
file(MAKE_DIRECTORY "./out/bin")    # Binary files (piboot, etc.)
file(MAKE_DIRECTORY "./out/img")    # Kernel image files (bootloader/kernel.img, etc.)

# Resolve absolute pathes to these directories
get_filename_component(LIB_OUT_DIR out/lib REALPATH)
get_filename_component(EXE_OUT_DIR out/bin REALPATH)
get_filename_component(IMG_OUT_DIR out/img REALPATH)
get_filename_component(PIBOOT_DIR PiBoot REALPATH)

set(CMAKE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/cmake)

########################################
#> End Project Setup                  <#
########################################

########################################
# Submodule Setup                      #
########################################

add_subdirectory(lib/startx86)
add_subdirectory(lib/libtypes)
add_subdirectory(lib/libmemory)
add_subdirectory(lib/libboot)
add_subdirectory(lib/libpi)

add_subdirectory(modules/bootloader)
add_subdirectory(modules/blink)

add_subdirectory(piboot)

########################################
#> End Submodule Setup                <#
########################################

########################################
# Metatarget Setup                     #
########################################

# Target to build all targets; added to default for GNU make.
add_custom_target(build_all ALL DEPENDS pi)


if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ARM")
    add_dependencies(build_all bootloader)
else ()
    add_dependencies(build_all piboot)
endif ()


########################################
#> End Metatarget Setup               <#
########################################