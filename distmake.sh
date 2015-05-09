VERSION="0.4.3"
make dist-xz
cp pdfmelange-$VERSION.tar.xz ~/tmp
cd ~/tmp
tar xvf pdfmelange-$VERSION.tar.xz
cd pdfmelange-$VERSION
dh_make -f ../pdfmelange-$VERSION.tar.xz
dpkg-buildpackage -S -us -uc
cd ..
ls -la