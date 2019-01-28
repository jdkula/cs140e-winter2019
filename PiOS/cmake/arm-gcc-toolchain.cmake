##########
# CS140E: ARM GCC Toolchain
# Jonathan Kula (via GitHub)
# Winter 2019
#
# This toolchain is courtesy of:
# >>  https://github.com/vpetrigo/arm-cmake-toolchains/blob/master/arm-gcc-toolchain.cmake
#
# This file provides an ARM compilation toolchain for CMake.

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

if(MINGW OR CYGWIN OR WIN32)
    set(UTIL_SEARCH_CMD where)
elseif(UNIX OR APPLE)
    set(UTIL_SEARCH_CMD which)
endif()

set(TOOLCHAIN_PREFIX arm-none-eabi-)

execute_process(
        COMMAND ${UTIL_SEARCH_CMD} ${TOOLCHAIN_PREFIX}gcc
        OUTPUT_VARIABLE BINUTILS_PATH
        OUTPUT_STRIP_TRAILING_WHITESPACE
)

get_filename_component(ARM_TOOLCHAIN_DIR ${BINUTILS_PATH} DIRECTORY)
# Without that flag CMake is not able to pass test compilation check
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)

set(CMAKE_ASM_LINKER_PREFERENCE ${TOOLCHAIN_PREFIX}ld)
set(CMAKE_C_LINK_EXECUTABLE "${TOOLCHAIN_PREFIX}ld <OBJECTS> <FLAGS> <LINK_FLAGS>  -o <TARGET> <LINK_LIBRARIES>")

set(CMAKE_OBJCOPY ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}objcopy CACHE INTERNAL "objcopy tool")
set(CMAKE_SIZE_UTIL ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}size CACHE INTERNAL "size tool")

set(CMAKE_FIND_ROOT_PATH ${BINUTILS_PATH})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)