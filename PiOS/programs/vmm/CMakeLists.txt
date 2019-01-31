##########
# CS140E: Replay
# Jonathan Kula
# Winter 2019
#
# Transparently vmms serial connections.
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

    project(vmm C)

    ########################################
    # Build Variable Setup                 #
    ########################################

    # Source files
    set(SRC
            src/main.c
            src/emulate-rpi.c
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

    set(DEPENDENCIES libpreprocessor piboot.lib vmm.lib)

    ## Output is controlled by ${EXE_OUT_DIR}

    ########################################
    #> End Build Variable Setup           <#
    ########################################

    ########################################
    # Execuable Target Setup               #
    ########################################

    # Create executable target with the given source files
    add_executable(vmm ${SRC})

    # Commit compile options
    target_compile_options(vmm PUBLIC ${COMPILE_OPTIONS})
    target_link_libraries(vmm ${DEPENDENCIES})

    target_include_directories(vmm PRIVATE ${INCLUDE_DIRECTORIES})

    set_target_properties(vmm
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${EXE_OUT_DIR}"
            )

    ########################################
    #> End Executable Target Setup        <#
    ########################################
endif ()