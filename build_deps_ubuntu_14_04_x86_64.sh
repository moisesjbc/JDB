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
);

echo "This script will install the following packages from repositories: "
echo ""
echo "${PACKAGES[@]}"
echo ""
echo "This script will also install manually SFML (2.3.2), m2g (0.3)"
echo "and TGUI (v0.7-alpha2) manually from Github in a \"third-party\""
echo "directory"
echo ""



EXECUTE_WITHOUT_ASKING_USER=0
while getopts ":y" opt; do
    echo "Parsing"
  case $opt in
    y)
      EXECUTE_WITHOUT_ASKING_USER=1
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      ;;
  esac
done


INSTALL=0
if [[ $EXECUTE_WITHOUT_ASKING_USER == 1 ]]
then
    INSTALL=1
else
    read -p "Install? (y/n) " -n 1 -r
    echo    # (optional) move to a new line
    if [[ $REPLY =~ ^[Yy]$ ]]
    then
        INSTALL=1
    fi
fi


if [[ $INSTALL == 1 ]]
then
    N_PROCESSORS=$(nproc)
    ROOT_DIR=$(pwd)/$(dirname $0)

    # Install dependencies from repositories.
    sudo apt-get install ${PACKAGES[@]} -y

    # Create temporal directory
    TEMP_DIR="$ROOT_DIR/temp-dependencies"
    mkdir -p $TEMP_DIR
    cd $TEMP_DIR

    # Start building dependencies and adding them to "third-party" dir.
    THIRD_PARTY_DIR="$ROOT_DIR/third-party/ubuntu_14_04_x86_64"

    CMAKE_ARGUMENTS="-DCMAKE_INSTALL_PREFIX=$THIRD_PARTY_DIR -DCMAKE_INCLUDE_PATH=$THIRD_PARTY_DIR/include -DCMAKE_LIBRARY_PATH=$THIRD_PARTY_DIR/lib -DCMAKE_BUILD_TYPE=Release"
    
    # Install SFML (v2.3.2) from source.
    wget http://www.sfml-dev.org/files/SFML-2.3.2-sources.zip
    unzip SFML-2.3.2-sources.zip
    cd SFML-2.3.2
    cmake $CMAKE_ARGUMENTS .
    sudo make install -j $N_PROCESSORS
    sudo ldconfig
    cd $TEMP_DIR

    # ADD SFML path to CMake arguments.
    CMAKE_ARGUMENTS="$CMAKE_ARGUMENTS -DCMAKE_MODULE_PATH=$THIRD_PARTY_DIR/share/SFML/cmake/Modules/ -DSFML_ROOT=$THIRD_PARTY_DIR"

    # Install TGUI (v0.7-dev) from source.
    wget https://github.com/texus/TGUI/archive/v0.7-alpha2.zip
    unzip v0.7-alpha2.zip
    cd TGUI-0.7-alpha2
    cmake $CMAKE_ARGUMENTS
    sudo make install -j $N_PROCESSORS
    sudo ldconfig
    cd $TEMP_DIR

    # Install m2g (v0.3.0) from source.
    wget https://github.com/moisesjbc/m2g/archive/v0.3.0.zip
    unzip v0.3.0.zip
    cd m2g-0.3.0/build
    cmake $CMAKE_ARGUMENTS
    sudo make install -j $N_PROCESSORS
    cd $TEMP_DIR
    sudo ldconfig

    # Strip generated libraries.
    for library in `ls $THIRD_PARTY_DIR/lib/li*.so.*`
    do
        sudo strip "$library"
    done

    # Destroy temporal directory
    cd ..
    sudo rm -r $TEMP_DIR

    # Copile Google Test
    cd /usr/src/gtest/
    sudo cmake .
    sudo make
    sudo mv libg* /usr/lib/
fi


# References
############
#
# How do I prompt a user for confirmation in bash script? [duplicate] - Stack Overflow
# http://stackoverflow.com/questions/1885525/how-do-i-prompt-a-user-for-confirmation-in-bash-script
# 
# The Ultimate Bash Array Tutorial with 15 Examples
# http://www.thegeekstuff.com/2010/06/bash-array-tutorial/
#
# Why no library files installed for google test? - Stack Overflow
# http://askubuntu.com/questions/145887/why-no-library-files-installed-for-google-test
