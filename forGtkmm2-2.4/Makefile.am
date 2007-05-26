#m4 macros needed to configure, link boost
ACLOCAL_AMFLAGS = -I m4
# program we want built
bin_PROGRAMS = peless

#DEPS_CFLAGS from gtkmm, others boost
peless_CXXFLAGS = $(DEPS_CFLAGS) $(BOOST_CPPFLAGS)
#boost link flags
peless_LDFLAGS = $(BOOST_LDFLAGS)
# gtkmm, boost libraries.
peless_LDADD=$(DEPS_LIBS)  $(BOOST_FILESYSTEM_LIB) $(BOOST_REGEX_LIB)

# sources required for peless
peless_SOURCES = peless.cc gmore.cc search.cc gmore.hh inittime.hh internat.hh search.hh

man1_MANS = peless.1

#extra files
EXTRA_DIST = philo.txt INTERNATIONALIZATION    peless.png   peless.spec.fc6  peless.1 peless.spec.man2007 peless.spec.OpenSuSE-10.2 peless.spec.data.OpenSuSE-10.2   peless.spec.data.fc6   peless.spec.data.man2007 peless.spec.fixed m4/ax_boost_base.m4 m4/ax_boost_filesystem.m4 m4/ax_boost_regex.m4


#clean this created file after each make
CLEANFILES=peless.desktop peless.spec.OpenSuSE-10.2 peless.spec.version peless.spec.fc6 peless.spec.man2007

# place peless.png in its proper place.
pixmapdir = @datadir@/pixmaps
pixmap_DATA = peless.png

# create and place peless.desktop
desktopdir = @datadir@/applications
desktop_DATA = peless.desktop
peless.desktop:
	echo "[Desktop Entry]" >peless.desktop
	echo "Encoding=UTF-8" >>peless.desktop
	echo "Name=peless" >>peless.desktop
	echo "Exec=@prefix@/bin/peless" >>peless.desktop
	echo -n "Icon=" >>peless.desktop
	echo -n @datadir@ >>peless.desktop
	echo "/pixmaps/peless.png" >>peless.desktop
	echo "Terminal=0" >>peless.desktop
	echo "StartupNotify=true" >>peless.desktop
	echo "Categories=Application;Utility;" >>peless.desktop

peless.spec.version:
	echo "%define _version @PACKAGE_VERSION@" >peless.spec.version
	echo -n "%define _release 0." >>peless.spec.version
	date -u +%y%m%d >>peless.spec.version


peless.spec.OpenSuSE-10.2:	peless.spec.version peless.spec.fixed peless.spec.data.OpenSuSE-10.2
	cat peless.spec.version peless.spec.data.OpenSuSE-10.2 peless.spec.fixed >peless.spec.OpenSuSE-10.2

peless.spec.fc6:	peless.spec.version peless.spec.fixed peless.spec.data.fc6
	cat peless.spec.version peless.spec.data.fc6 peless.spec.fixed >peless.spec.fc6

peless.spec.man2007:	peless.spec.version peless.spec.fixed peless.spec.data.man2007
	cat peless.spec.version peless.spec.data.man2007 peless.spec.fixed >peless.spec.man2007


 dist-hook:	peless.spec.OpenSuSE-10.2 peless.spec.fc6 peless.spec.man2007

