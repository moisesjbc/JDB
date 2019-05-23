#!/bin/bash

SFML_VERSION=$1
THIRD_PARTY_DIR=$2

CMAKE_ARGUMENTS="-DCMAKE_INSTALL_PREFIX=$THIRD_PARTY_DIR -DCMAKE_INCLUDE_PATH=$THIRD_PARTY_DIR/include -DCMAKE_LIBRARY_PATH=$THIRD_PARTY_DIR/lib -DCMAKE_BUILD_TYPE=Release"

# Install SFML from source.
wget https://www.sfml-dev.org/files/SFML-$SFML_VERSION-sources.zip
unzip SFML-$SFML_VERSION-sources.zip
cd SFML-$SFML_VERSION
cmake $CMAKE_ARGUMENTS .
make install
sudo ldconfig
cd $TEMP_DIR
