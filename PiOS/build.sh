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

### ARM ###
# Clean and create temporary build directory
rm -rf /tmp/jdkula_pios_build_arm
mkdir /tmp/jdkula_pios_build_arm
cd /tmp/jdkula_pios_build_arm

# Generate project
cmake -DCMAKE_TOOLCHAIN_FILE=${PROJECT_DIR}/cmake/arm-gcc-toolchain.cmake ${PROJECT_DIR}

# Compile project
make

# Return to original working directory
cd ${ORIGINAL_DIR}

mv out out-arm

# Remove temporary directory.
rm -rf /tmp/jdkula_pios_build_arm

### X86 ###
# Clean and create temporary build directory
rm -rf /tmp/jdkula_pios_build_arm
mkdir /tmp/jdkula_pios_build_arm
cd /tmp/jdkula_pios_build_arm

# Generate project
cmake ${PROJECT_DIR}

# Compile project
make

# Return to original working directory
cd ${ORIGINAL_DIR}

mv out out-x86

# Remove temporary directory.
rm -rf /tmp/jdkula_pios_build_arm