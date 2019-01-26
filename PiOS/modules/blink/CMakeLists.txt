##########
# CS140E: Bootloader
# Jonathan Kula
# Winter 2019
#
# This kernel works with the PiBoot executable to load
# arbitrary kernel images using a UART connection; this
# eliminates the need to switch out the kernel on the SD
# card with each build, speeding up development.
#
# This build file was last updated on January 25th, 2019.
# If you're unfamiliar with CMake, you can just use the
# build.sh file present at the project root to build all.
#
# This project should not be generated directly; use the
# CMakeLists.txt file located at the root of the project.

cmake_minimum_required(VERSION 3.10)

########################################
# Project Setup                        #
########################################

project(blink C ASM)
set(MODULE_NAME blink)

########################################
#> End Project Setup                  <#
########################################

########################################
# Build Variable Setup                 #
########################################

# Source Files
set(SRC src/blink.c)
if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ARM")
    list(APPEND SRC src/start.s)
endif ()

# Compile Options
if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ARM")
    set(COMPILE_OPTIONS -nostdlib -nostartfiles -ffreestanding -fno-builtin)
else ()
    set(COMPILE_OPTIONS)
endif ()

set(INCLUDE_DIRECTORIES include)

# Dependencies
set(BOOTLOADER_DEPENDENCIES pi boot)  # Link to libpi and boot

# Linker Options
if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ARM")
    set(LINKER_SCRIPT linker/memmap)
    get_filename_component(LINKER_SCRIPT_ABSOLUTE "${LINKER_SCRIPT}" REALPATH)

    set(LINKER_OPTIONS -T${LINKER_SCRIPT_ABSOLUTE} -nostdlib)
else ()
    set(LINKER_OPTIONS)
endif ()


# Output Options
## Raw Binary goes to ${EXE_OUT_DIR} (see root CMakeLists.txt)
## See ${IMG_OUT_DIR} in root CMakeLists.txt
set(IMAGE_OUTPUT_DIRECTORY "${IMG_OUT_DIR}/${MODULE_NAME}")

########################################
#> End Build Variable Setup           <#
########################################

include(${CMAKE_INCLUDE_DIR}/module.cmake)