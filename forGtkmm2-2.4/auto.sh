#!/bin/bash

if [[ ! -e intltool-extract.in ]] ; then
echo getting intltool-extract.in
cp /usr/share/intltool/intltool-extract.in .
fi
if [[ ! -e intltool-update.in ]] ; then
echo getting intltool/intltool-update.in
cp /usr/share/intltool/intltool-update.in .
fi
if [[ ! -e intltool-merge.in ]] ; then
echo getting intltool-merge.in
cp /usr/share/intltool/intltool-merge.in .
fi
if [[ ! -e po/Makefile.in.in ]] ; then
echo getting po/Makefile.in.in
cp /usr/share/intltool/Makefile.in.in po
fi

if [[ true ]] 
then

gnome-autogen.sh

else

echo doing glib_gettextize
glib-gettextize --force --copy
echo doing intltoolize
intltoolize --copy --force --automake
echo doing autoheader
autoheader 
touch NEWS README AUTHORS ChangeLog
touch stamp-h
echo doing aclocal
aclocal -I /opt/gnome/share/aclocal -I /usr/share/aclocal -I ./m4
echo doing autoconf
autoconf
echo doing automake
if [[ -e missing ]] ; then
automake
else
automake -ac
fi



fi

echo doing ./configure
./configure
echo doing make
make
echo doing make dist_bzip2
make dist-bzip2