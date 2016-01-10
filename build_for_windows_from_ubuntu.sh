cd /var/tmp

# Prepare MXE

git clone https://github.com/mxe/mxe.git && 
sudo mv mxe /opt/mxe && 
cd /opt/mxe && 
make MXE_TARGETS='i686-w64-mingw32.static' gcc sfml tinyxml2 boost &&
echo 'export PATH=/opt/mxe/usr/bin:$PATH' >> ~/.bashrc

# Download FindSFML.cmake

wget https://raw.githubusercontent.com/SFML/SFML/master/cmake/Modules/FindSFML.cmake && 
mv FindSFML.cmake /opt/mxe/usr/share/cmake/modules/

# Cross compile m2g

wget https://github.com/moisesjbc/m2g/archive/master.zip --output-document m2g-master.zip &&
unzip m2g-master.zip &&
cd m2g-master/build &&
i686-w64-mingw32.static-cmake . -DSFML_ROOT=/opt/mxe/usr/i686-w64-mingw32.static/ -DSFML_STATIC_LIBRARIES=1 -DBUILD_SHARED_LIBS=False -DBUILD_STATIC_M2G=1 &&
make -j 6 &&
mv lib/libm2g.a /opt/mxe/usr/i686-w64-mingw32.static/lib/ &&
cd ../..

# Cross compile tgui

wget https://github.com/texus/TGUI/archive/v0.7-alpha2.zip --output-document tgui.zip &&
unzip tgui.zip &&
cd TGUI-0.7-alpha2/ &&
i686-w64-mingw32.static-cmake . -DSFML_ROOT=/opt/mxe/usr/i686-w64-mingw32.static/ -DSFML_STATIC_LIBRARIES=1 -DTGUI_SHARED_LIBS=False &&
make -j 6 &&
sudo mv lib/libtgui-s.a /opt/mxe/usr/i686-w64-mingw32.static/lib/ &&
cd ..
