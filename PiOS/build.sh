#!/bin/bash

## FROM https://stackoverflow.com/questions/59895/getting-the-source-directory-of-a-bash-script-from-within
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"
## END FROM

rm -rf /tmp/jdkula_pios_build
mkdir /tmp/jdkula_pios_build
cd /tmp/jdkula_pios_build

cmake -DCMAKE_TOOLCHAIN_FILE=${DIR}/arm-gcc-toolchain.cmake ${DIR}
make build_all

cd ${DIR}
rm -rf /tmp/jdkula_pios_build