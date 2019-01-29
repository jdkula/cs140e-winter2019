##########
# CS140E: PiBoot
# Jonathan Kula
# Winter 2019
#
# A relatively simple bootloader client for the Raspberry Pi.
# Connects to the Raspberry Pi over a TTY serial connection,
# and pushes a given kernel file to it.
#
# This build file was last updated on January 27th, 2019.
# If you're unfamiliar with CMake, you can just use the
# build.sh file present at the project root to build all.
#
# This project may be generated directly, but it is recommended to use the
# CMakeLists.txt file located at the root of the project.

# Only build on x86
if (NOT ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ARM")

    cmake_minimum_required(VERSION 3.10)

    ########################################
    # Project Setup                        #
    ########################################

    project(PiBoot C)

    ########################################
    # Build Variable Setup                 #
    ########################################

    # Source files
    set(SRC
            src/my-install.c
            src/fatal.c
            src/simple-boot.c
            src/support.c
            src/trace.c
            src/tty.c
            )

    # Compile options
    set(COMPILE_OPTIONS
            -Wall
            -Og
            -ggdb
            -Iinclude
            -g
            -lm)

    set(INCLUDE_DIRECTORIES include)

    set(DEPENDENCIES libboot)

    ## Output is controlled by ${EXE_OUT_DIR}

    ########################################
    #> End Build Variable Setup           <#
    ########################################

    ########################################
    # Execuable Target Setup               #
    ########################################

    # Create executable target with the given source files
    add_executable(piboot ${SRC})

    # Commit compile options
    target_compile_options(piboot PUBLIC ${COMPILE_OPTIONS})
    target_link_libraries(piboot ${DEPENDENCIES})

    target_include_directories(piboot PRIVATE ${INCLUDE_DIRECTORIES})

    set_target_properties(piboot
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${EXE_OUT_DIR}"
            )

    add_library(piboot.lib STATIC ${SRC})

    target_compile_options(piboot.lib PUBLIC ${COMPILE_OPTIONS})
    target_link_libraries(piboot.lib ${DEPENDENCIES})
    target_include_directories(piboot.lib PUBLIC ${INCLUDE_DIRECTORIES})
    set_target_properties(piboot.lib
            PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUT_DIR}"
            )

    ########################################
    #> End Executable Target Setup        <#
    ########################################
endif ()