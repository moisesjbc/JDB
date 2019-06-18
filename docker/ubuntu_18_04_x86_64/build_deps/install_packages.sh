#!/bin/bash

declare -a PACKAGES=( 
    'binutils'
    'build-essential'
    'cmake'
    'wget'
    'unzip'
    'libtinyxml2-dev'
    'libfreetype6-dev'
    'libjpeg-dev'
    'libx11-dev'
    'x11-xserver-utils'
    'libx11-xcb-dev'
    'libxcb-randr0-dev'
    'libxcb-image0-dev'
    'libgl1-mesa-dev'
    'libglu1-mesa-dev'
    'libflac-dev'
    'libogg-dev'
    'libvorbis-dev'
    'libvorbisenc2'
    'libvorbisfile3'
    'libopenal-dev'
    'libpthread-stubs0-dev'
    'libboost-filesystem-dev'
    'libboost-system-dev'
    'glew-utils'
    'libgl1-mesa-dev'
    'libudev-dev'
    'pkg-config'
    'libgtest-dev'
    'xorg-dev'
    'gcc-7'
    'g++-7'
);

# Install dependencies from repositories.
sudo apt-get update
sudo apt install ${PACKAGES[@]} -y

