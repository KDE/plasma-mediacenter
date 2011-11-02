#! /usr/bin/env bash
$XGETTEXT `find . -name \*.qml` -L Java -o $podir/plasma_applet_org.kde.mediacontainment.pot
rm -f rc.cpp
