#! /usr/bin/env bash
$EXTRACTRC `find . -name \*.ui` >> rc.cpp
$XGETTEXT `find . -name \*qml` -j -L Java -o $podir/org.kde.mediacontainment.pot
rm -f rc.cpp
