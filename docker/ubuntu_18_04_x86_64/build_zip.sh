#!/bin/bash

TMP_DIR=/tmp/sandwiches-game
SRC_DIR=/opt/sandwiches-game
ZIP_FILENAME="sandwiches-game-v1.1.1.zip"
ZIP_FILEPATH="/tmp/$ZIP_FILENAME"
DIST_DIR=$SRC_DIR/dist

apt install zip

cmake -DCMAKE_CXX_FLAGS="-I$THIRD_PARTY_DIR/include -L$THIRD_PARTY_DIR/lib" .
make -j $N_PROC
(rm -rf $TMP_DIR || exit 0)
mkdir -p $TMP_DIR/lib
mkdir $TMP_DIR/bin
cp $THIRD_PARTY_DIR/lib/*.so* $TMP_DIR/lib
cp $SRC_DIR/bin/sandwiches-game $TMP_DIR/bin
cp -r $SRC_DIR/share/sandwiches-game $TMP_DIR/data
cp /opt/launcher.sh $TMP_DIR/sandwiches-game.sh
chmod +x $TMP_DIR/sandwiches-game.sh

chown -R $HOST_UID:$HOST_GID $TMP_DIR
chroot -R go+w $TMP_DIR
(cd /tmp && zip -r $ZIP_FILENAME sandwiches-game)
(mkdir $DIST_DIR || exit 0)

chown -R $HOST_UID:$HOST_GID $ZIP_FILEPATH
chown -R $HOST_UID:$HOST_GID $DIST_DIR

mv $ZIP_FILEPATH $DIST_DIR

