# program we want built
bin_PROGRAMS = peless

# sources required for peless
peless_SOURCES = peless.cc gmore.cc search.cc gmore.hh inittime.hh internat.hh search.hh

man1_MANS = peless.1

#extra files
EXTRA_DIST = philo.txt INTERNATIONALIZATION peless.spec.FedoraCore2 peless.spec.SuSE.90 peless.spec.SuSE.92 peless.png peless.spec.ubuntu.510 ubuntu.510/control ubuntu.510/dirs ubuntu.510/docs ubuntu.510/rules peless.spec.SuSE.93 peless.spec.SuSE.10.0 peless.spec.SuSE.10.1 peless.spec.FedoraCore5 peless.spec.man2006 peless.spec.FedoraCore6 peless.spec.SuSE.scott peless.1 peless.spec.man2007 peless.spec.OpenSuSE-10.2

# to compile, use flags needed by both gtkmm and boost
peless_CXXFLAGS=$(DEPS_CFLAGS) $(BOOST_CXXFLAGS)
# in above, DEPS_CFLAGS and BOOST_CXXFLAGS came from configure.ac

# to link, use flags needed by both gtkmm and boost
peless_LDADD=$(DEPS_LIBS) $(BOOST_LIBS)
# in above, DEPS_LIBS and BOOST_LIBS came from configure.ac

#clean this created file after each make
CLEANFILES=peless.desktop peless.spec.OpenSuSE-10.2 peless.spec.version

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


peless.spec.OpenSuSE-10.2:	peless.spec.version peless.spec.fixed
	cat peless.spec.version peless.spec.data.OpenSuSE-10.2 peless.spec.fixed >peless.spec.OpenSuSE-10.2

 dist-hook:	peless.spec.OpenSuSE-10.2

