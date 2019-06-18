FROM ubuntu:18.04
MAINTAINER Moisés J. <moisesjbc@gmail.com>

# Update Ubuntu 18.08.
RUN export DEBIAN_FRONTEND=noninteractive
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install sudo

# Set environment variables.
ENV N_PROC 6
ENV SFML_VERSION 2.5.1
ENV TGUI_VERSION 0.8.5
ENV M2G_VERSION 0.3.0
ENV TEMP_DIR /var/tmp/third-party
ENV SCRIPTS_DIR /var/tmp/build_deps
ENV THIRD_PARTY_DIR /opt/third-party

# Copy build scripts to Docker.
COPY ./build_deps $SCRIPTS_DIR
WORKDIR $SCRIPTS_DIR
RUN chmod +x $SCRIPTS_DIR/*.sh

# Install dependencies from Ubuntu repositories.
RUN ./install_packages.sh

# Prepare a directory for dependencies.
RUN mkdir $THIRD_PARTY_DIR

# Install SFML from source.
RUN $SCRIPTS_DIR/install_sfml.sh $SFML_VERSION $THIRD_PARTY_DIR $TEMP_DIR $N_PROC

# Install TGUI from source.
RUN $SCRIPTS_DIR/install_tgui.sh $TGUI_VERSION $THIRD_PARTY_DIR $TEMP_DIR $N_PROC

# Install m2g from source.
RUN $SCRIPTS_DIR/install_m2g.sh $M2G_VERSION $THIRD_PARTY_DIR $TEMP_DIR $N_PROC

# Copy tinyxml2 library to third-party dir.
RUN cp /usr/lib/x86_64-linux-gnu/libtinyxml2.so* $THIRD_PARTY_DIR/lib

# Copy boost libraries to third-party dir.
RUN cp /usr/lib/x86_64-linux-gnu/libboost_* $THIRD_PARTY_DIR/lib

# Strip generated libraries.
RUN for library in `ls $THIRD_PARTY_DIR/lib/li*.so.*`; do sudo strip "$library"; done

# Create directory for sharing game source.
RUN mkdir /opt/sandwiches-game
WORKDIR /opt/sandwiches-game

COPY ./build_zip.sh /usr/bin/build_zip.sh
RUN chmod +x /usr/bin/build_zip.sh

COPY ./launcher.sh /opt/launcher.sh
