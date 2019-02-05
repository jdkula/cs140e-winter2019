##########
# CS140E: LibPi
# Jonathan Kula
# Winter 2019
#
# Provides interfaces for Raspberry Pi hardware, like
# GPIO, UART, timing, rebooting, and so on.
#
# This build file was last updated on January 27th, 2019.
# If you're unfamiliar with CMake, you can just use the
# build.sh file present at the project root to build all.
#
# This project should not be generated directly; use the
# CMakeLists.txt file located at the root of the project.

cmake_minimum_required(VERSION 3.10)

########################################
# Project Setup                        #
########################################

project(libpi C ASM)
set(LIBRARY_NAME libpi)

########################################
#> End Project Setup                  <#
########################################

########################################
# Build Variable Setup                 #
########################################

# Source Files
list_add_sources_multiarch(SRC
        aux.c
        gpio.c
        reboot.c
        timer.c
        uart.c
        mem-access.c
        printf.c
        dummy.s
        dummy.c
        mem-access.c
        mem-access.s
        mem-barrier.c
        mem-barrier.s
        mem-x86-extensions.c)

set(SRC_VMM
        common/gpio.c
        common/reboot.c
        common/timer.c
        common/uart.c
        common/mem-access.c
        x86/mem-x86-extensions.c
        x86/mem-access.c
        x86/mem-barrier.c
        vmm/aux.c
        vmm/vmm.c)

# Compile & Link Options
set(INCLUDE_DIRECTORIES include-common)

if (NOT ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ARM")
    list(APPEND INCLUDE_DIRECTORIES include-x86)
endif ()

# Dependencies
set(DEPENDENCIES libtypes libpreprocessor)

########################################
#> End Build Variable Setup           <#
########################################

include(${CMAKE_INCLUDE_DIR}/library.cmake)