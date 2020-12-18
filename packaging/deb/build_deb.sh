#/bin/bash

NAME=qjournalctl
VERSION=0.6.2
PKGREV=1
OUT="$NAME""_$VERSION-$PKGREV"

# Build
QT_SELECT=qt5 qmake /source/qjournalctl.pro -r -spec linux-g++ CONFIG+=release
make -j$(nproc)

# Move required files
cp -r /source/packaging/files "$OUT/"
mkdir -p "$OUT/usr/bin"
mv qjournalctl "$OUT/usr/bin"

# debian pkg files
mkdir -p "$OUT/DEBIAN/"
cp /source/packaging/deb/control "$OUT/DEBIAN/"
dpkg-deb --build "$OUT"

# Keep ownership
mv "$OUT.deb" "/out/$OUT.deb"
chown $USERID:$GROUPID "/out/$OUT.deb"
