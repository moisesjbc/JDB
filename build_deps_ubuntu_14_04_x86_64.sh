#!/bin/bash

# Create temporal directory
mkdir temp-dependencies
cd temp-dependencies

# Install binutils and cmake
sudo apt-get install binutils cmake

# Install tinyxml (v2.0)
sudo apt-get install libtinyxml2-dev

# TODO: Install SFML dependencies
sudo apt-get install libfreetype6-dev libjpeg-dev libx11-dev x11-xserver-utils libx11-xcb-dev libxcb-randr0-dev libxcb-image0-dev libgl1-mesa-dev libglu1-mesa-dev libflac-dev libogg-dev libvorbis-dev libvorbisenc2 libvorbisfile3 libopenal-dev libpthread-stubs0-dev

# Install SFML (v2.3)
wget http://www.sfml-dev.org/files/SFML-2.3-linux-gcc-64-bit.tar.gz
tar xvzf SFML-2.3-linux-gcc-64-bit.tar.gz
sudo cp -R SFML-2.3/* /usr/local

# Install m2g (v0.3.0)
wget https://github.com/moisesjbc/m2g/archive/v0.3.0.zip
unzip v0.3.0.zip
cd m2g-0.3.0/build
cmake .
sudo make install
cd ../..

# Install TGUI (v0.7-dev)
wget https://github.com/texus/TGUI/archive/v0.7-dev.zip
unzip v0.7-dev.zip
cd TGUI-0.7-dev/
cmake .
sudo make install
cd ..

# Update dynamic linker.
sudo ldconfig

# Destroy temporal directory
cd ..
sudo rm -r temp-dependencies
