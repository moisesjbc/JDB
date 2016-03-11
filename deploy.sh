#!/bin/bash

# Build DEB package for Ubuntu
make clean &&
sudo rm -r CMakeCache.txt CMakeFiles &&
cmake -DCMAKE_BUILD_TYPE=Release . &&
sudo make -j $(nproc) package

# Build ZIP package for Windows
make clean &&
sudo rm -r CMakeCache.txt CMakeFiles &&
i686-w64-mingw32.static-cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_USING_MXE=1 . &&
sudo make -j $(nproc) package
