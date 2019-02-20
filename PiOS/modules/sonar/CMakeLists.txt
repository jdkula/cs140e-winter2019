##########
# CS140E: Snar
# Jonathan Kula
# Winter 2019
#
# This kernel works with the sonar module [INSERT-MODULE-HERE]
# to print out the distance to the given objet.
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

project(sonar C ASM)
set(MODULE_NAME sonar)

########################################
#> End Project Setup                  <#
########################################

########################################
# Build Variable Setup                 #
########################################

# Source Files
set(SRC src/sonar.c)

# Compile Options
set(DEPENDENCIES libpi libdebug libpreprocessor libstart)
set(INCLUDE_DIRECTORIES include)


# Output Options
## Raw Binary goes to ${EXE_OUT_DIR} (see root CMakeLists.txt)
## See ${IMG_OUT_DIR} in root CMakeLists.txt
set(IMAGE_OUTPUT_DIRECTORY "${IMG_OUT_DIR}/${MODULE_NAME}")

########################################
#> End Build Variable Setup           <#
########################################

include(${CMAKE_INCLUDE_DIR}/module.cmake)
