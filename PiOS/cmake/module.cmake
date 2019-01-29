##########
# CS140E: Module CMake Snippet
# Jonathan Kula
# Winter 2019
#
# This file provides a list of commands
# that set up a RPi kernel.img module from a bunch
# of environment variables.
#
# This file should be included in the project's CMakeLists.txt
# This file expends the following environment variables to be present:
# MODULE_NAME - The name of the module
# SRC - List of source files
# SYSTEM_INCLUDE_DIRECTORIES [optional] - List of directories to include as system includes.
# INCLUDE_DIRECTORIES [optional] - List of directories to include
# DEFINES [optional] - List of macro definitions to include.
# COMPILE_OPTIONS [optional] - List of options to give to the copiler
# LINK_OPTIONS [optional] - List of options to give to the linker
# LINKER_SCRIPT [optional] - A linker script to use (if existient).
# DEPENDENCIES [optional] - List of libraries to link to.
# EXE_OUT_DIR - Location to place the output executable file.
# IMAGE_OUTPUT_DIRECTORY - Location to place the output .img, .hex, and .list files.

########################################
# File Resolution                      #
########################################

get_filename_component(LINKER_SCRIPT_ABSOLUTE "${LINKER_SCRIPT}" REALPATH)

#message(STATUS "+-+-+-+-+ Building ${MODULE_NAME} +-+-+-+-+")
#message(STATUS "+ MODULE_NAME - ${MODULE_NAME}")
#message(STATUS "+ SRC - ${SRC}")
#message(STATUS "+ SYSTEM_INCLUDE_DIRECTORIES - ${SYSTEM_INCLUDE_DIRECTORIES}")
#message(STATUS "+ INCLUDE_DIRECTORIES - ${INCLUDE_DIRECTORIES}")
#message(STATUS "+ DEFINES - ${DEFINES}")
#message(STATUS "+ COMPILE_OPTIONS - ${COMPILE_OPTIONS}")
#message(STATUS "+ LINK_OPTIONS - ${LINK_OPTIONS}")
#message(STATUS "+ LINKER_SCRIPT - ${LINKER_SCRIPT}")
#message(STATUS "+ LINKER_SCRIPT_ABSOLUTE - ${LINKER_SCRIPT_ABSOLUTE}")
#message(STATUS "+ DEPENDENCIES - ${DEPENDENCIES}")
#message(STATUS "+ EXE_OUT_DIR - ${EXE_OUT_DIR}")
#message(STATUS "+ IMAGE_OUTPUT_DIRECTORY - ${IMAGE_OUTPUT_DIRECTORY}")
#message(STATUS "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+")

if (EXISTS "${LINKER_SCRIPT_ABSOLUTE}")
    list(APPEND LINK_OPTIONS "-T${LINKER_SCRIPT_ABSOLUTE}")
endif ()

########################################
#> End File Resolution                <#
########################################

########################################
# Executable Setup                     #
########################################

# Create executable target with predefined source
add_executable(${MODULE_NAME}.exec ${SRC})

# Commit compile options
target_compile_options(${MODULE_NAME}.exec PRIVATE ${COMPILE_OPTIONS})
target_include_directories(${MODULE_NAME}.exec SYSTEM PUBLIC ${SYSTEM_INCLUDE_DIRECTORIES})
target_include_directories(${MODULE_NAME}.exec PUBLIC ${INCLUDE_DIRECTORIES})

if (NOT "${DEFINES}" MATCHES ".*MODULE_NAME.*")
    list(APPEND DEFINES "MODULE_NAME=${MODULE_NAME}")
endif ()

target_compile_definitions(${MODULE_NAME}.exec PRIVATE ${DEFINES})

# Link dependencies and add linker arguments (target_link_options only supported in CMake >3.13)
target_link_libraries(${MODULE_NAME}.exec ${DEPENDENCIES} start ${LINK_OPTIONS})

# Set other properties
set_target_properties(${MODULE_NAME}.exec
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${EXE_OUT_DIR}"
        )

# Ensure output directory exists
file(MAKE_DIRECTORY "${IMAGE_OUTPUT_DIRECTORY}")


########################################
#> End Executable Setup               <#
########################################

########################################
# Library Setup                        #
########################################
# This allows testing frameworks to link with the module.

set(LIBRARY_NAME ${MODULE_NAME}.lib)
include(${CMAKE_INCLUDE_DIR}/library.cmake)

########################################
#> End Library Setup                  <#
########################################

########################################
# Metatarget Setup                     #
########################################
# Target that does all the objdump/objcopy chicanery to convert the base binary to a kernel image with associated metadata.
# Build this to build the kernel image.
add_custom_target(${MODULE_NAME} "${CMAKE_OBJDUMP}" -D $<TARGET_FILE:${MODULE_NAME}.exec> > "${IMAGE_OUTPUT_DIRECTORY}/${MODULE_NAME}.list"  # Objdump/asm list
        COMMAND "${CMAKE_OBJCOPY}" $<TARGET_FILE:${MODULE_NAME}.exec> -O ihex "${IMAGE_OUTPUT_DIRECTORY}/${MODULE_NAME}.hex"  # Hex representation of binary
        COMMAND "${CMAKE_OBJCOPY}" $<TARGET_FILE:${MODULE_NAME}.exec> -O binary "${IMAGE_OUTPUT_DIRECTORY}/kernel.img"  # Create the image file
        DEPENDS ${MODULE_NAME}.exec ${MODULE_NAME}.lib  # Make sure we've built everything first!
        )