##########
# CS140E: PiShell Server
# Jonathan Kula
# Winter 2019
#
# This kernel provides a "server" end of the Pi Shell.
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

project(pishell-server C ASM)
set(MODULE_NAME pishell-server)

########################################
#> End Project Setup                  <#
########################################

########################################
# Build Variable Setup                 #
########################################

# Source Files
set(SRC src/bootloader.c src/pi-shell.c)

# Compile Options
set(DEPENDENCIES libpi libdebug libpreprocessor libstart libboot)
set(INCLUDE_DIRECTORIES include)


# Output Options
## Raw Binary goes to ${EXE_OUT_DIR} (see root CMakeLists.txt)
## See ${IMG_OUT_DIR} in root CMakeLists.txt
set(IMAGE_OUTPUT_DIRECTORY "${IMG_OUT_DIR}/${MODULE_NAME}")

########################################
#> End Build Variable Setup           <#
########################################

include(${CMAKE_INCLUDE_DIR}/module.cmake)
