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

project(libtypes C)

########################################
#> End Project Setup                  <#
########################################

########################################
# Build Variable Setup                 #
########################################

if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ARM")
    set(INCLUDE_DIRECTORIES include-arm)
else ()
    set(INCLUDE_DIRECTORIES include-x86)
endif ()

########################################
#> End Build Variable Setup           <#
########################################

########################################
# Library Setup                        #
########################################

# Create static library target from predefined source
add_library(libtypes INTERFACE)

# Add include directories and share them with dependants.
target_include_directories(libtypes INTERFACE ${INCLUDE_DIRECTORIES})

########################################
#> End Library Setup                  <#
########################################