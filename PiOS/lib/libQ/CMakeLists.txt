##########
# CS140E: LibQ
# Jonathan Kula
# Winter 2019
#
# Provides a really basic FIFO Queue.
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

project(libq C)
set(LIBRARY_NAME libq)

########################################
#> End Project Setup                  <#
########################################

########################################
# Build Variable Setup                 #
########################################

# Compile & Link Options
set(INCLUDE_DIRECTORIES include)

# Dependencies
set(DEPENDENCIES libtypes)

########################################
#> End Build Variable Setup           <#
########################################

# Create static library target from predefined source
add_library(${LIBRARY_NAME} INTERFACE)

# Add include directories and share them with dependants.
target_include_directories(${LIBRARY_NAME} INTERFACE ${INCLUDE_DIRECTORIES})
