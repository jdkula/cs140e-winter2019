##########
# CS140E: StartX86
# Jonathan Kula
# Winter 2019
#
# Provides a [main] that launches module_main.
# Used to be interoperable with existing ARM code
# that provides a _start method.
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

project(start C)
set(LIBRARY_NAME start)

########################################
#> End Project Setup                  <#
########################################

if (NOT ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ARM")

    ########################################
    # Build Variable Setup                 #
    ########################################

    # Source Files
    set(SRC x86/start.c)

    set(INCLUDE_DIRECTORIES include-common)


    ########################################
    #> End Build Variable Setup           <#
    ########################################

    include(${CMAKE_INCLUDE_DIR}/library.cmake)


else ()

    # Add a dummy target that does nothing. Start functionality should be provided by start.s.
    add_library(start INTERFACE)

endif ()