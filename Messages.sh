#!bin/sh
 
$XGETTEXT `find . ! -path "./shells/next/*" \( -name \*.cc -o -name \*.cpp -o -name \*.h -o -name \*.qml \)` -o $podir/plasma-mediacenter.pot

