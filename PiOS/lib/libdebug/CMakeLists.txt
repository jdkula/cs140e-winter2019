##########
# CS140E: LibDebug
# Jonathan Kula
# Winter 2019
#
# Provides some helper functions that trigger various
# GPIO pins set to LEDs, and are easily disableable.
#
# This build file was last updated on January 27th, 2019.
# If you're unfamiliar with CMake, you can just use the
# build.sh file present at the project root to build all.
#
# This project should not be generated directly; use the
# CMakeLists.txt file located at the root of the project.

cmake_minimum_required(VERSION 3.10)

### ! ### LIBDEBUG CAN BE DISABLED BY UNCOMMENTING THE BELOW VALUE!
#set(DEFINES DEBUG_OFF)

########################################
# Project Setup                        #
########################################

project(libdebug C)
set(LIBRARY_NAME libdebug)

########################################
#> End Project Setup                  <#
########################################


########################################
# Build Variable Setup                 #
########################################

# Source Files

list_add_sources_multiarch(SRC debug.c)

set(INCLUDE_DIRECTORIES include)

set(DEPENDENCIES libtypes)

if ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "ARM")
    list(APPEND DEPENDENCIES libpi)
endif ()


########################################
#> End Build Variable Setup           <#
########################################

include(${CMAKE_INCLUDE_DIR}/library.cmake)