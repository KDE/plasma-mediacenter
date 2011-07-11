#! /usr/bin/env bash
$EXTRACTRC `find . -name \*.ui` >> rc.cpp
$XGETTEXT `find . -name \*qml` -j -L Java -o $podir/plasma-containment-mediacontainment.pot
rm -f rc.cpp
