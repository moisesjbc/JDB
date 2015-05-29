#!/bin/bash

declare -a PACKAGES=( 
    'binutils'
    'cmake'
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
    'libboost-filesystem1.55-dev'
    'libboost-system1.55-dev'
);

echo "This script will install the following packages from repositories: "
echo ""
echo "${PACKAGES[@]}"
echo ""
echo "This script will also install SFML (2.3), m2g (0.3) and TGUI 
(v0.7-dev) from Github"
echo ""

read -p "Install? (y/n) " -n 1 -r
echo    # (optional) move to a new line
if [[ $REPLY =~ ^[Yy]$ ]]
then
    # Install dependencies from repositories.
    sudo apt-get install ${PACKAGES[@]}

    # Create temporal directory
    mkdir temp-dependencies
    cd temp-dependencies

    # Install SFML (v2.3) from source.
    wget http://www.sfml-dev.org/files/SFML-2.3-linux-gcc-64-bit.tar.gz
    tar xvzf SFML-2.3-linux-gcc-64-bit.tar.gz
    sudo cp -R SFML-2.3/* /usr/local

    # Install m2g (v0.3.0) from source.
    wget https://github.com/moisesjbc/m2g/archive/v0.3.0.zip
    unzip v0.3.0.zip
    cd m2g-0.3.0/build
    cmake .
    sudo make install
    cd ../..

    # Install TGUI (v0.7-dev) from source.
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
fi


# References
############
#
# How do I prompt a user for confirmation in bash script? [duplicate] - Stack Overflow
# http://stackoverflow.com/questions/1885525/how-do-i-prompt-a-user-for-confirmation-in-bash-script
# 
# The Ultimate Bash Array Tutorial with 15 Examples
# http://www.thegeekstuff.com/2010/06/bash-array-tutorial/
