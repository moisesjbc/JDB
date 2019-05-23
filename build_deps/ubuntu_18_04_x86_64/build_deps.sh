#!/bin/bash

SFML_VERSION=2.5.1
TGUI_VERSION=0.8.5
M2G_VERSION=0.3.0

echo "This script will install the following packages from repositories: "
echo ""
echo "${PACKAGES[@]}"
echo ""
echo "This script will also install manually SFML (v$SFML_VERSION), m2g (v$M2G_VERSION)"
echo "and TGUI (v$TGUI_VERSION) manually from Github in a \"third-party\""
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
    SCRIPTS_DIR=$(pwd)/$(dirname $0)
    ROOT_DIR=$SCRIPTS_DIR/../..

    bash $SCRIPTS_DIR/install_packages.sh

    # Create temporal directory
    TEMP_DIR="$ROOT_DIR/temp-dependencies"
    (rm -r $TEMP_DIR || exit 0)
    mkdir -p $TEMP_DIR
    cd $TEMP_DIR

    # Start building dependencies and adding them to "third-party" dir.
    THIRD_PARTY_DIR="$ROOT_DIR/third-party/ubuntu_18_04_x86_64"
   
    # Install SFML from source.
    bash $SCRIPTS_DIR/install_sfml.sh $SFML_VERSION $THIRD_PARTY_DIR

    # Install TGUI from source.
    bash $SCRIPTS_DIR/install_tgui.sh $SFML_VERSION $THIRD_PARTY_DIR $CMAKE_ARGUMENTS

    # Install m2g from source.
    bash $SCRIPTS_DIR/install_m2g.sh $M2G_VERSION $THIRD_PARTY_DIR

    # Strip generated libraries.
    for library in `ls $THIRD_PARTY_DIR/lib/li*.so.*`
    do
        sudo strip "$library"
    done

    # Destroy temporal directory
    cd ..
    rm -r $TEMP_DIR

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
