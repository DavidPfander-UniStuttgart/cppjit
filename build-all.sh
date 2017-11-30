#!/bin/bash
set -e
set -x

source source-me.sh

cd $BASE_PATH

./scripts/build-cmake.sh

git pull
mkdir -p build
cd build
cmake -DCMAKE_INSTALL_PREFIX="$CPPJIT_ROOT" ../
make -j4 VERBOSE=1 install
