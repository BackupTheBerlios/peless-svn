autoheader -B/home/pelliott/install/gnome2/include
touch NEWS README AUTHORS ChangeLog
touch stamp-h
aclocal
autoconf
automake
./configure
make
