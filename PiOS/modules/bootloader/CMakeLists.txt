##########
# CS140E: Bootloader
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

if ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "ARM")

    ########################################
    # Project Setup                        #
    ########################################

    project(bootloader C ASM)
    set(MODULE_NAME bootloader)

    ########################################
    #> End Project Setup                  <#
    ########################################

    ########################################
    # Build Variable Setup                 #
    ########################################

    # Source Files
    set(SRC src/cstart.c src/bootloader.c)
    if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ARM")
        list(INSERT SRC 0 src/start.s)  # start.s must be built first.
    endif ()

    # Compile Options
    set(DEPENDENCIES libpi libboot libdebug libpreprocessor)
    set(INCLUDE_DIRECTORIES include)


    # Output Options
    ## Raw Binary goes to ${EXE_OUT_DIR} (see root CMakeLists.txt)
    ## See ${IMG_OUT_DIR} in root CMakeLists.txt
    set(IMAGE_OUTPUT_DIRECTORY "${IMG_OUT_DIR}/${MODULE_NAME}")

    ########################################
    #> End Build Variable Setup           <#
    ########################################

    include(${CMAKE_INCLUDE_DIR}/module.cmake)

endif ()