#!/bin/bash

declare -a PACKAGES=( 
    'binutils'
    'build-essential'
    'cmake3'
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
    'gcc-4.9' # Ubuntu 18.04 installs gcc / g++ 4.8 but we need 4.9 for TGUI
    'g++-4.9'
);

# Install dependencies from repositories.
sudo apt install -y software-properties-common # Needed for add-apt-repository
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y # PPA for gcc / g++ 4.9
sudo apt-get update
sudo apt install ${PACKAGES[@]} -y

sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.9 60 --slave /usr/bin/g++ g++ /usr/bin/g++-4.9

# Source for gcc / g++ 4.9 installation: https://gist.github.com/goldsborough/d466f43e8ffc948ff92de7486c5216d6
