#!/bin/sh

qmake journalctlgui.pro -r -spec linux-g++ CONFIG+=release

if [ -d ".git" ]; then
    sed -i -e 's/[[:xdigit:]]\{7\}/'$(git log --pretty=format:'%h' -n 1)'/' src/version.h
else
    sed -i -e 's/[[:xdigit:]]\{7\}/0000000/' src/version.h
fi



