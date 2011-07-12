#! /usr/bin/env bash
$XGETTEXT `find . -name \*.qml` -L Java -o $podir/org.kde.mediacontainment.pot
rm -f rc.cpp
