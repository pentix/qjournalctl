#!/bin/sh

# See Issue #38 -- Choose qmake first
QMAKE_BIN="$(which qmake)"
if [ ! -f "$QMAKE_BIN" ]; then
    # -- If it's not there, try qmake-qt5 second
    QMAKE_BIN="$(which qmake-qt5)"
fi

if [ ! -f "$QMAKE_BIN" ]; then
    # If it's still not there...
    echo "Did not find qmake. Please make sure either"
    echo "qmake or qmake-qt5 is provided by your system."
    exit 1
fi

QT_SELECT=qt5 $QMAKE_BIN qjournalctl.pro -r -spec linux-g++ CONFIG+=release

if [ -d ".git" ]; then
    sed -i -e 's/[[:xdigit:]]\{7\}/'$(git log --pretty=format:'%h' -n 1)'/' src/version.h
else
    sed -i -e 's/[[:xdigit:]]\{7\}/0000000/' src/version.h
fi



