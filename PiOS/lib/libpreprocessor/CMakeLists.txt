##########
# CS140E: LibPreprocessor
# Jonathan Kula
# Winter 2019
#
# Contains useful preprocessor macros
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

project(libpreprocessor C)
set(LIBRARY_NAME libpreprocessor)

########################################
#> End Project Setup                  <#
########################################

########################################
# Build Variable Setup                 #
########################################

set(INCLUDE_DIRECTORIES include)

########################################
#> End Build Variable Setup           <#
########################################

########################################
# Library Setup                        #
########################################

add_library(${LIBRARY_NAME} INTERFACE)

# Add include directories and share them with dependants.
target_include_directories(${LIBRARY_NAME} INTERFACE ${INCLUDE_DIRECTORIES})

# Commit compile and link options
target_compile_options(${LIBRARY_NAME} INTERFACE ${COMPILE_OPTIONS})


########################################
#> End Library Setup                  <#
########################################)