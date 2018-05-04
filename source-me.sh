#!/bin/bash
set -e
set -x

echo "in cppjit source-me"

if [[ -z "$PARALLEL_BUILD" ]]; then
    #use all available CPUs
    export PARALLEL_BUILD=$((`lscpu -p=cpu | wc -l`-4))
fi

REL_BASE_PATH=`dirname "$BASH_SOURCE"`
BASE_PATH=`readlink -f $REL_BASE_PATH`


export CPPJIT_ROOT=$BASE_PATH/cppjit_install

export PATH=$BASE_PATH/cmake_install/bin:$PATH
echo "PATH in cppjit: $PATH"
