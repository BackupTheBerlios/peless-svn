autoheader 
touch NEWS README AUTHORS ChangeLog
touch stamp-h
aclocal
autoconf
if [[ -e missing ]] ; then
automake
else
automake -ac
fi
./configure
make
make dist-bzip2