#!/bin/bash

set -e

# Build directory management
if [ ! -d `pwd`/build ]; then
    mkdir `pwd`/build
else
    rm -rf `pwd`/build/*
fi

# output directory management
if [ ! -d `pwd`/bin ]; then
    mkdir `pwd`/bin
else
    rm -rf `pwd`/bin/web*
fi

# Build with CMake
cd `pwd`/build &&
    cmake .. &&
    make -j$(nproc)  # Use all CPU cores

