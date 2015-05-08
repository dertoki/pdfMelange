#!/bin/bash

PREFIX='/opt/gtk38/'
HOST='i686-w64-mingw32'
BUILD='x86_64-linux-gnu'

# website: http://poppler.freedesktop.org/
#URL_poppler='http://poppler.freedesktop.org/poppler-0.24.5.tar.xz'
#DIR_poppler='poppler-0.24.5'
URL_poppler='http://poppler.freedesktop.org/poppler-0.32.0.tar.xz'
DIR_poppler='poppler-0.32.0'

export CFLAGS="-I$PREFIX/include -Os -s"
export CPPFLAGS="-I$PREFIX/include -Os -s"
export CXXFLAGS="-I$PREFIX/include -Os -s"
export LDFLAGS="-L$PREFIX/lib"
export PKG_CONFIG_LIBDIR="$PREFIX/lib/pkgconfig"


wget $URL_poppler
tar xvf $(basename $URL_poppler)
cd $DIR_poppler

./configure --build=$BUILD --host=$HOST --prefix=$PREFIX --enable-xpdf-headers
make
#sudo make install
cd ..
