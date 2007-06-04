autoheader 
touch NEWS README AUTHORS ChangeLog
touch stamp-h
aclocal -I /opt/gnome/share/aclocal -I /usr/share/aclocal -I ./m4
autoconf
if [[ -e missing ]] ; then
automake
else
automake -ac
fi
./configure
make
make dist-bzip2