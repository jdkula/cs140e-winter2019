##########
# CS140E: gprof
# Jonathan Kula
# Winter 2019
#
# A simple test program using interrupts to periodically
# poll where the program is at.
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

project(thread-test C ASM)
set(MODULE_NAME thread-test)

########################################
#> End Project Setup                  <#
########################################

########################################
# Build Variable Setup                 #
########################################

# Source Files
set(SRC src/thread-test.c)

# Compile Options

# Dependencies
set(DEPENDENCIES libpi libdebug libstart)

# Output Options
## Raw Binary goes to ${EXE_OUT_DIR} (see root CMakeLists.txt)
## See ${IMG_OUT_DIR} in root CMakeLists.txt
set(IMAGE_OUTPUT_DIRECTORY "${IMG_OUT_DIR}/${MODULE_NAME}")

########################################
#> End Build Variable Setup           <#
########################################

include(${CMAKE_INCLUDE_DIR}/module.cmake)