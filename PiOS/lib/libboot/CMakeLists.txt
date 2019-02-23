##########
# CS140E: LibBoot
# Jonathan Kula
# Winter 2019
#
# Provides src code and the interface used by the
# bootloader module and PiBoot, the client of the bootloader module.
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

project(libboot C)
set(LIBRARY_NAME libboot)

########################################
#> End Project Setup                  <#
########################################

########################################
# Build Variable Setup                 #
########################################

# Source Files
list_add_sources_multiarch(SRC crc32.c boot.c send.c support.c)

# Compile Options
set(INCLUDE_DIRECTORIES include)

# Dependencies
if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ARM")
    set(DEPENDENCIES libtypes libpi)
else ()
    set(DEPENDENCIES libtypes libtty libpreprocessor)
endif ()

########################################
#> End Build Variable Setup           <#
########################################

# Library Target Setup
include(${CMAKE_INCLUDE_DIR}/library.cmake)
