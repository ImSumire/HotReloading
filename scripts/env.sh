#!/bin/sh

# Include Raylib 5.5.0 package
mkdir -p pkg
cd pkg

if test -f "./raylib/README.md";
then
    echo "Found pkg/raylib/"
else
    git clone --shallow-submodules --recursive --depth 1 https://github.com/raysan5/raylib.git
fi

# Build Raylib
cd raylib

if test -f "./build/raylib/libraylib.so";
then
    echo "Found libraylib.so"
else
    echo "Building raylib..." &&
    mkdir -p build &&
    cd build &&
    cmake .. -GNinja -DBUILD_SHARED_LIBS=ON -DWITH_PIC=ON &&
    ninja &&
    cd ..
fi

# Back to the root
cd ../..

mkdir -p dist

cp pkg/raylib/build/raylib/libraylib.so.??? dist
