##########
# CS140E: Replay
# Jonathan Kula
# Winter 2019
#
# Transparently replays serial connections.
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

    project(replay C)

    ########################################
    # Build Variable Setup                 #
    ########################################

    # Source files
    set(SRC
            src/parse-input.c
            src/Q.c
            src/replay-driver.c
            src/replay.c
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

    set(DEPENDENCIES libpreprocessor piboot.lib)

    ## Output is controlled by ${EXE_OUT_DIR}

    ########################################
    #> End Build Variable Setup           <#
    ########################################

    ########################################
    # Execuable Target Setup               #
    ########################################

    # Create executable target with the given source files
    add_executable(replay ${SRC})

    # Commit compile options
    target_compile_options(replay PUBLIC ${COMPILE_OPTIONS})
    target_link_libraries(replay ${DEPENDENCIES})

    target_include_directories(replay PRIVATE ${INCLUDE_DIRECTORIES})

    set_target_properties(replay
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${EXE_OUT_DIR}"
            )

    ########################################
    #> End Executable Target Setup        <#
    ########################################
endif ()