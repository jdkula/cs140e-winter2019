#!/bin/bash

ORIGINAL_DIR=$(pwd)

## FROM https://stackoverflow.com/questions/59895/getting-the-source-directory-of-a-bash-script-from-within
# Get directory of of this script.
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  PROJECT_DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ ${SOURCE} != /* ]] && SOURCE="$PROJECT_DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
PROJECT_DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"
## END FROM

### CMAKE CHECK ###
CMAKE_VER=$(cmake --version | grep -oE "[0-9]+\.[0-9]+")
CMAKE_STAT=$?
CMAKE_MAJOR=$(echo ${CMAKE_VER} | cut -d'.' -f1 - )
CMAKE_MINOR=$(echo ${CMAKE_VER} | cut -d'.' -f2 - )

[[ CMAKE_MAJOR -ge 3 ]] && [[ CMAKE_MINOR -ge 10 ]]
CMAKE_OK=$?

if [[ ${CMAKE_STAT} -ne 0 ]] || [[ ${CMAKE_OK} -ne 0 ]]; then
    if [[ ! -f /tmp/jdkula_pios_cmake/.has_cmake ]]; then
        echo "CMake wasn't found. Downloading to /tmp/jdkula_pios_cmake."
        rm -rf /tmp/jdkula_pios_cmake
        mkdir /tmp/jdkula_pios_cmake
        cd /tmp/jdkula_pios_cmake
        wget -q --show-progress https://github.com/Kitware/CMake/releases/download/v3.13.3/cmake-3.13.3-Linux-x86_64.tar.gz -O cmake.tar.gz
        tar --strip-components=1 -xvzf cmake.tar.gz > /dev/null
        touch .has_cmake
    fi
    export CMAKE=/tmp/jdkula_pios_cmake/bin/cmake
else
    export CMAKE=cmake
fi

### ARM ###
# Clean and create temporary build directory
rm -rf /tmp/jdkula_pios_build_arm
mkdir /tmp/jdkula_pios_build_arm
cd /tmp/jdkula_pios_build_arm

echo ===== BUILDING ARM VERSION ======
# Generate project
${CMAKE} -DCMAKE_TOOLCHAIN_FILE=${PROJECT_DIR}/cmake/arm-gcc-toolchain.cmake ${PROJECT_DIR}

# Compile project
make

# Return to original working directory
cd ${ORIGINAL_DIR}

# Remove temporary directory.
rm -rf /tmp/jdkula_pios_build_arm

### X86 ###
# Clean and create temporary build directory
rm -rf /tmp/jdkula_pios_build_x86
mkdir /tmp/jdkula_pios_build_x86
cd /tmp/jdkula_pios_build_x86

echo ===== BUILDING X86 VERSION ======
# Generate project
${CMAKE} ${PROJECT_DIR}

# Compile project
make

# Return to original working directory
cd ${ORIGINAL_DIR}

# Remove temporary directory.
rm -rf /tmp/jdkula_pios_build_x86

echo Output is located in ./out-x86 and ./out-arm