##########
# CS140E: LibBoot
# Jonathan Kula
# Winter 2019
#
# Provides src code and the interface used by the
# bootloader module and PiBoot, the client of the bootloader module.
#
# This build file was last updated on January 27th, 2019.
# If you're unfamiliar with CMake, you can just use the
# build.sh file present at the project root to build all.
#
# This project should not be generated directly; use the
# CMakeLists.txt file located at the root of the project.

cmake_minimum_required(VERSION 3.10)

if ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "ARM")

    ########################################
    # Project Setup                        #
    ########################################


    project(libboot-impl C)
    set(LIBRARY_NAME libboot-impl)

    ########################################
    #> End Project Setup                  <#
    ########################################

    ########################################
    # Build Variable Setup                 #
    ########################################

    # Source Files
    set(SRC sysspec/arm/boot.c)

    # Compile Options
    set(INCLUDE_DIRECTORIES include)

    # Dependencies
    set(DEPENDENCIES libtypes libboot libpi)


    ########################################
    #> End Build Variable Setup           <#
    ########################################

    # Library Target Setup
    include(${CMAKE_INCLUDE_DIR}/library.cmake)

endif ()