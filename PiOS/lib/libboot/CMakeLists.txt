##########
# CS140E: LibBoot
# Jonathan Kula
# Winter 2019
#
# Provides common code and the interface used by the
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
set(SRC common/crc32.c)

# Compile Options
set(INCLUDE_DIRECTORIES include-common)

# Dependencies
set(DEPENDENCIES libtypes)


########################################
#> End Build Variable Setup           <#
########################################

# Library Target Setup
include(${CMAKE_INCLUDE_DIR}/library.cmake)
