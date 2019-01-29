##########
# CS140E: StartX86
# Jonathan Kula
# Winter 2019
#
# Provides a [main] that launches module_main.
# Used to be interoperable with existing ARM code
# that provides a _start method.
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

project(start C)
set(LIBRARY_NAME start)

########################################
#> End Project Setup                  <#
########################################

if (NOT ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ARM")

    ########################################
    # Build Variable Setup                 #
    ########################################

    # Source Files
    get_filename_component(START_FULL x86/start.c REALPATH)

    set(DEPENDENCIES libpreprocessor)

    ########################################
    #> End Build Variable Setup           <#
    ########################################

    ########################################
    # Library Setup                        #
    ########################################

    add_library(${LIBRARY_NAME} INTERFACE)

    target_sources(${LIBRARY_NAME} INTERFACE ${START_FULL})

    # Add include directories and share them with dependants.
    target_include_directories(${LIBRARY_NAME} INTERFACE ${INCLUDE_DIRECTORIES})

    # Link libraries
    target_link_libraries(${LIBRARY_NAME} INTERFACE ${DEPENDENCIES})

    # Commit compile and link options
    target_compile_options(${LIBRARY_NAME} INTERFACE ${COMPILE_OPTIONS})


    ########################################
    #> End Library Setup                  <#
    ########################################


else ()

    # Add a dummy target that does nothing. Start functionality should be provided by start.s.
    add_library(start INTERFACE)

endif ()