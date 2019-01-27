##########
# CS140E: LibMemory
# Jonathan Kula
# Winter 2019
#
# Provides interfaces for Raspberry Pi memory, like
# get32, set32, etc. This prevents GCC from optimizing
# memory code unexpectedly, and allows for a drop-in test
# framework that can be run on the computer.
# Also provides memory barriers.
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

project(libmemory C ASM)
set(LIBRARY_NAME libmemory)

########################################
#> End Project Setup                  <#
########################################

########################################
# Build Variable Setup                 #
########################################

# Source Files
set(SRC common/mem-access.c)
if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ARM")
    list(APPEND SRC
            arm/mem-access.c
            arm/mem-access.s
            arm/mem-barrier.s
            )
else ()
    list(APPEND SRC
            x86/mem-access.c
            x86/mem-barrier.c
            )
endif ()


# Compile Options
set(INCLUDE_DIRECTORIES include-common)

# Dependencies
set(DEPENDENCIES libtypes)

########################################
#> End Build Variable Setup           <#
########################################

########################################
# Library Setup                        #
########################################

include(${CMAKE_INCLUDE_DIR}/library.cmake)

########################################
#> End Library Setup                  <#
########################################