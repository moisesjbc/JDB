#!/bin/bash

# Build DEB package for Ubuntu
cmake . &&
make -j $(nproc) package

# Build ZIP package for Windows
make clean &&
sudo rm -r CMakeCache.txt CMakeFiles &&
i686-w64-mingw32.static-cmake -DBUILD_USING_MXE=1 . &&
make -j $(nproc) package
