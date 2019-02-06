##########
# CS140E: LibTTY
# Jonathan Kula
# Winter 2019
#
# Provides functions on x86 for interacting with TTY (e.g. UART)
#
# This build file was last updated on January 27th, 2019.
# If you're unfamiliar with CMake, you can just use the
# build.sh file present at the project root to build all.
#
# This project should not be generated directly; use the
# CMakeLists.txt file located at the root of the project.

cmake_minimum_required(VERSION 3.10)


if (NOT "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "ARM")

    ########################################
    # Project Setup                        #
    ########################################

    project(libtty C)
    set(LIBRARY_NAME libtty)

    ########################################
    #> End Project Setup                  <#
    ########################################


    ########################################
    # Build Variable Setup                 #
    ########################################

    # Source Files
    set(SRC sysspec/x86/tty.c sysspec/x86/tty-comm.c)

    set(INCLUDE_DIRECTORIES include)

    set(DEPENDENCIES libpreprocessor libtypes)


    ########################################
    #> End Build Variable Setup           <#
    ########################################

    include(${CMAKE_INCLUDE_DIR}/library.cmake)
endif ()