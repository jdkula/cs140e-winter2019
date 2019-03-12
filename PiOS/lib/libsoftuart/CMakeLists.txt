##########
# CS140E: LibSoftUART
# Jonathan Kula
# Winter 2019
#
# Provides a bit-banging software UART.
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

project(libsoftuart C)
set(LIBRARY_NAME libsoftuart)

########################################
#> End Project Setup                  <#
########################################


########################################
# Build Variable Setup                 #
########################################

# Source Files

list_add_sources_multiarch(SRC softuart.c)

if ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "ARM")
    set(DEPENDENCIES libpi)
else ()
    set(DEPENDENCIES)
endif ()

set(INCLUDE_DIRECTORIES include)

########################################
#> End Build Variable Setup           <#
########################################

if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ARM")
    include(${CMAKE_INCLUDE_DIR}/library.cmake)
endif ()