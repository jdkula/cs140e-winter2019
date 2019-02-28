##########
# CS140E: PiFS
# Jonathan Kula
# Winter 2019
#
# A very simple FUSE file abstraction for PiShell.
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

    project(pifs C)
    include(FindPkgConfig)

    ########################################
    # Build Variable Setup                 #
    ########################################

    # Source files
    set(SRC
            src/pi-fs.c
            src/pi-fs-support.c
            src/redirect.c
            src/echo-until.c
            )

    pkg_check_modules(FUSE fuse)
    # Compile options
    set(COMPILE_OPTIONS
            -Wall
            -Og
            -ggdb
            -Iinclude
            -g
            -lm
            ${FUSE_CFLAGS})

    set(INCLUDE_DIRECTORIES include)

    set(DEPENDENCIES libboot libpreprocessor libtty piboot.lib ${FUSE_LDFLAGS})

    ## Output is controlled by ${EXE_OUT_DIR}

    ########################################
    #> End Build Variable Setup           <#
    ########################################

    ########################################
    # Execuable Target Setup               #
    ########################################

    # Create executable target with the given source files
    add_executable(pifs ${SRC})

    # Commit compile options
    target_compile_options(pifs PUBLIC ${COMPILE_OPTIONS})
    target_link_libraries(pifs ${DEPENDENCIES})

    target_include_directories(pifs PRIVATE ${INCLUDE_DIRECTORIES})

    set_target_properties(pifs
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${EXE_OUT_DIR}"
            )

    add_library(pifs.lib STATIC ${SRC})

    target_compile_options(pifs.lib PUBLIC ${COMPILE_OPTIONS})
    target_link_libraries(pifs.lib ${DEPENDENCIES})
    target_include_directories(pifs.lib PUBLIC ${INCLUDE_DIRECTORIES})
    set_target_properties(pifs.lib
            PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUT_DIR}"
            )


    add_executable(test-redirect-driver src/driver.c src/redirect.c src/echo-until.c )
    target_compile_options(test-redirect-driver PUBLIC ${COMPILE_OPTIONS})
    target_link_libraries(test-redirect-driver ${DEPENDENCIES})
    target_include_directories(test-redirect-driver PUBLIC ${INCLUDE_DIRECTORIES})

    add_executable(test-pi-shell-redirect src/redirect.c src/echo-until.c src/test-pi-shell-redirect.c)
    target_compile_options(test-pi-shell-redirect PUBLIC ${COMPILE_OPTIONS})
    target_link_libraries(test-pi-shell-redirect ${DEPENDENCIES})
    target_include_directories(test-pi-shell-redirect PUBLIC ${INCLUDE_DIRECTORIES})

    add_executable(test-redirect src/redirect.c src/test-redirect.c src/echo-until.c)
    target_compile_options(test-redirect PUBLIC ${COMPILE_OPTIONS})
    target_link_libraries(test-redirect ${DEPENDENCIES})
    target_include_directories(test-redirect PUBLIC ${INCLUDE_DIRECTORIES})



    ########################################
    #> End Executable Target Setup        <#
    ########################################
endif ()