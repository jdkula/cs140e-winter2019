##########
# CS140E: PiShell
# Jonathan Kula
# Winter 2019
#
# A very simple shell client to the PiShell module.
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

    project(pishell C)

    ########################################
    # Build Variable Setup                 #
    ########################################

    # Source files
    set(SRC
            src/pi-shell.c
            src/shell-support.c
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

    set(DEPENDENCIES libboot libpreprocessor libtty piboot.lib)

    ## Output is controlled by ${EXE_OUT_DIR}

    ########################################
    #> End Build Variable Setup           <#
    ########################################

    ########################################
    # Execuable Target Setup               #
    ########################################

    # Create executable target with the given source files
    add_executable(pishell ${SRC})

    # Commit compile options
    target_compile_options(pishell PUBLIC ${COMPILE_OPTIONS})
    target_link_libraries(pishell ${DEPENDENCIES})

    target_include_directories(pishell PRIVATE ${INCLUDE_DIRECTORIES})

    set_target_properties(pishell
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${EXE_OUT_DIR}"
            )

    add_library(pishell.lib STATIC ${SRC})

    target_compile_options(pishell.lib PUBLIC ${COMPILE_OPTIONS})
    target_link_libraries(pishell.lib ${DEPENDENCIES})
    target_include_directories(pishell.lib PUBLIC ${INCLUDE_DIRECTORIES})
    set_target_properties(pishell.lib
            PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUT_DIR}"
            )

    ########################################
    #> End Executable Target Setup        <#
    ########################################
endif ()