##########
# CS140E: VMM
# Jonathan Kula
# Winter 2019
#
# This kernel works with the PiBoot executable to load
# arbitrary kernel images using a UART connection; this
# eliminates the need to switch out the kernel on the SD
# card with each build, speeding up development.
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

project(vmm C ASM)
set(MODULE_NAME vmm)

########################################
#> End Project Setup                  <#
########################################

########################################
# Build Variable Setup                 #
########################################

# Source Files
set(SRC src/pi-vmm.c)

# Compile Options
set(DEPENDENCIES libpi libpreprocessor libstart)
set(INCLUDE_DIRECTORIES include)


# Output Options
## Raw Binary goes to ${EXE_OUT_DIR} (see root CMakeLists.txt)
## See ${IMG_OUT_DIR} in root CMakeLists.txt
set(IMAGE_OUTPUT_DIRECTORY "${IMG_OUT_DIR}/${MODULE_NAME}")

########################################
#> End Build Variable Setup           <#
########################################

include(${CMAKE_INCLUDE_DIR}/module.cmake)
