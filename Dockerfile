FROM ubuntu:14.04
MAINTAINER Moisés J. <moisesjbc@gmail.com>

RUN export DEBIAN_FRONTEND=noninteractive
RUN apt-get update
RUN apt-get upgrade -y

COPY build_deps/ubuntu_18_04_x86_64 /var/tmp/build_deps

ENV SFML_VERSION 2.5.1
ENV TGUI_VERSION 0.8.5
ENV M2G_VERSION 0.3.0
ENV SCRIPTS_DIR /var/tmp/build_deps
ENV THIRD_PARTY_DIR $SCRIPTS_DIR/third-party

WORKDIR /var/tmp/build_deps
RUN chmod +x $SCRIPTS_DIR/*.sh
RUN mkdir $THIRD_PARTY_DIR

# Install dependencies from Ubuntu repositories.
RUN ./install_packages.sh

# Install SFML from source.
RUN $SCRIPTS_DIR/install_sfml.sh $SFML_VERSION $THIRD_PARTY_DIR

# Install TGUI from source.
RUN $SCRIPTS_DIR/install_tgui.sh $TGUI_VERSION $THIRD_PARTY_DIR

# Install m2g from source.
RUN $SCRIPTS_DIR/install_m2g.sh $M2G_VERSION $THIRD_PARTY_DIR

# Strip generated libraries.
RUN for library in `ls $THIRD_PARTY_DIR/lib/li*.so.*`; do sudo strip "$library"; done

WORKDIR /var/tmp/sandwiches-game

ENTRYPOINT cp /usr/lib/x86_64-linux-gnu/libtinyxml2.so.0.0.0 third-party/ubuntu_18_04_x86_64/lib && \
    cp -R /var/tmp/build_deps/third-party/* third-party/ubuntu_18_04_x86_64/ && \
    rm -rf CMakeFiles/ CMakeCache.txt && \
    cmake . && \
    make -j 6

# sudo docker run -v `pwd`:/var/tmp/sandwiches-game -it moisesjbc/ubuntu-base-for-sandwiches-game
# rm -rf CMakeFiles/ CMakeCache.txt
# cmake .
# make package -j 6
# cp /usr/lib/x86_64-linux-gnu/libtinyxml2.so.0.0.0 third-party/ubuntu_18_04_x86_64/lib
# cp -R /var/tmp/build_deps/third-party/* third-party/ubuntu_18_04_x86_64/

# In root
# LD_LIBRARY_PATH=third-party/ubuntu_18_04_x86_64/lib/ bin/sandwiches-game
