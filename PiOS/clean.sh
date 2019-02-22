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

find ${PROJECT_DIR}/out* -type f -exec rm -fv {} \;
find ${PROJECT_DIR}/cmake-build* -type f -exec rm -fv {} \;