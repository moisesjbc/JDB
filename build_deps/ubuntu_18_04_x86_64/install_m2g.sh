#!/bin/bash

M2G_VERSION=$1
THIRD_PARTY_DIR=$2

wget https://github.com/moisesjbc/m2g/archive/v$M2G_VERSION.zip
unzip v$M2G_VERSION.zip
cd m2g-$M2G_VERSION/build
cmake $CMAKE_ARGUMENTS
make install
sudo ldconfig
