autoheader -B/home/pelliott/install/gnome2/include
touch NEWS README AUTHORS ChangeLog
touch stamp-h
aclocal
autoconf
if [[ -e missing ]] ; then
automake
else
automake -a
fi
./configure
make
