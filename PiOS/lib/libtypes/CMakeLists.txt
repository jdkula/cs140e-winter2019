##########
# CS140E: LibTypes
# Jonathan Kula
# Winter 2019
#
# Provides consistient typing between the Pi and Computer.
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

project(libtypes C)

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

# Create static library target from predefined source
add_library(libtypes INTERFACE)

# Add include directories and share them with dependants.
target_include_directories(libtypes INTERFACE ${INCLUDE_DIRECTORIES})

########################################
#> End Library Setup                  <#
########################################