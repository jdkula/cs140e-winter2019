##########
# CS140E: LibTest
# Jonathan Kula
# Winter 2019
#
# Provides some test harnesses that may be useful.
#
# This build file was last updated on January 27th, 2019.
# If you're unfamiliar with CMake, you can just use the
# build.sh file present at the project root to build all.
#
# This project should not be generated directly; use the
# CMakeLists.txt file located at the root of the project.

cmake_minimum_required(VERSION 3.10)


if (NOT "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "ARM")

    ########################################
    # Project Setup                        #
    ########################################

    project(libtest C)
    set(LIBRARY_NAME libtest)

    ########################################
    #> End Project Setup                  <#
    ########################################


    ########################################
    # Build Variable Setup                 #
    ########################################

    # Source Files
    set(SRC x86/test-memory.c x86/test-memory-handlers.c)

    set(INCLUDE_DIRECTORIES include-x86)

    set(DEPENDENCIES libpi libtypes)


    ########################################
    #> End Build Variable Setup           <#
    ########################################

    include(${CMAKE_INCLUDE_DIR}/library.cmake)
endif ()