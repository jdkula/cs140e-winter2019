##########
# CS140E: LibPi
# Jonathan Kula
# Winter 2019
#
# Provides interfaces for Raspberry Pi hardware, like
# GPIO, UART, timing, rebooting, and so on.
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

project(libpi C ASM)
set(LIBRARY_NAME libpi)

########################################
#> End Project Setup                  <#
########################################

########################################
# Build Variable Setup                 #
########################################

# Source Files
set(SRC_ARM
        sysspec/arm/interrupts.S
        src/interrupts.c
        src/arm.c
        src/aux.c
        src/gpio.c
        src/mem-access.c
        src/printf.c
        src/reboot.c
        src/timer.c
        src/uart.c
        src/thread.c
        sysspec/arm/thread-asm.s
        sysspec/arm/dummy.s
        sysspec/arm/mem-access.c
        sysspec/arm/mem-access.s
        sysspec/arm/mem-barrier.s
        sysspec/arm/stdlib.c
        sysspec/arm/heap.c
        sysspec/arm/rand.c
        sysspec/arm/vm.c
        sysspec/arm/your-vm-asm.S
        src/interrupt-vectors.c)

set(SRC_VMM
        src/gpio.c
        src/reboot.c
        src/timer.c
        src/uart.c
        src/mem-access.c
        src/printf.c
#        src/interrupts.c
#        src/arm.c
        sysspec/x86/vmm-memory-barriers.c
        sysspec/x86/mem-x86-extensions.c
        sysspec/x86/aux-x86-extensions.c
        sysspec/x86/dummy.c
        sysspec/x86/aux.c
        sysspec/x86/vmm.c
        sysspec/x86/vmm-memory.c
        sysspec/x86/rand.c
        sysspec/x86/thread-vmm.c)

# Compile & Link Options
set(INCLUDE_DIRECTORIES include)

# Dependencies
set(DEPENDENCIES libtypes libpreprocessor libq)
set(DEPENDENCIES_VMM ${DEPENDENCIES} libtty)

########################################
#> End Build Variable Setup           <#
########################################

if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ARM")
    add_library(${LIBRARY_NAME} STATIC ${SRC_ARM})

    # Add include directories and share them with dependants.
    target_include_directories(${LIBRARY_NAME} PUBLIC ${INCLUDE_DIRECTORIES})

    # Add target defines.
    target_compile_definitions(${LIBRARY_NAME} PRIVATE ${DEFINES} IS_LIBRARY)

    # Commit compile and link options
    target_compile_options(${LIBRARY_NAME} PUBLIC ${COMPILE_OPTIONS})
    target_link_libraries(${LIBRARY_NAME} ${DEPENDENCIES} ${LINK_OPTIONS})

    # Commit output directory
    set_target_properties(${LIBRARY_NAME}
            PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUT_DIR}"
            LIBRARY_OUTPUT_DIRECTORY "${LIB_OUT_DIR}"
            PREFIX ""
            )
else ()
    # Create static library target from predefined source
    add_library(${LIBRARY_NAME}-vmm STATIC ${SRC_VMM})

    # Add include directories and share them with dependants.
    target_include_directories(${LIBRARY_NAME}-vmm PUBLIC ${INCLUDE_DIRECTORIES})

    # Add target defines.
    target_compile_definitions(${LIBRARY_NAME}-vmm PRIVATE ${DEFINES} IS_LIBRARY)

    # Commit compile and link options
    target_compile_options(${LIBRARY_NAME}-vmm PUBLIC ${COMPILE_OPTIONS})
    target_link_libraries(${LIBRARY_NAME}-vmm ${DEPENDENCIES_VMM} ${LINK_OPTIONS})

    # Commit output directory
    set_target_properties(${LIBRARY_NAME}-vmm
            PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUT_DIR}"
            LIBRARY_OUTPUT_DIRECTORY "${LIB_OUT_DIR}"
            PREFIX ""
            )
endif ()