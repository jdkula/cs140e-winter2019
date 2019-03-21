##########
# CS140E: XBee Echo
# Jonathan Kula
# Winter 2019
#
# Echoes data between two XBees.
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

project(xbee-echo C ASM)
set(MODULE_NAME xbee-echo)

########################################
#> End Project Setup                  <#
########################################

########################################
# Build Variable Setup                 #
########################################

# Source Files
set(SRC src/cstart.c src/xbee-echo.c)
if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ARM")
    list(INSERT SRC 0 src/start.s)  # start.s must be built first.
endif ()

# Compile Options

# Dependencies
set(DEPENDENCIES libpi libdebug libsoftuart)

# Output Options
## Raw Binary goes to ${EXE_OUT_DIR} (see root CMakeLists.txt)
## See ${IMG_OUT_DIR} in root CMakeLists.txt
set(IMAGE_OUTPUT_DIRECTORY "${IMG_OUT_DIR}/${MODULE_NAME}")

########################################
#> End Build Variable Setup           <#
########################################

include(${CMAKE_INCLUDE_DIR}/module.cmake)