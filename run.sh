#!/bin/sh

set -e # Exit early if any commands fail

(
  cd "$(dirname "$0")" # Ensure compile steps are run within the repository directory
  cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake
  cmake --build ./build
)

if [ "$1" = "test" ]; then
  shift
  (
    cd "$(dirname "$0")/build"
    ctest --verbose "$@"
  )
  exit 0
fi

exec valgrind "$(dirname "$0")/build/sqlite" "$@"
