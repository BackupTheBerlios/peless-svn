#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.57)
 AC_INIT(peless, 0.51)
 AC_CONFIG_SRCDIR([peless.cc])
 AC_CONFIG_HEADER([config.h])
 AM_INIT_AUTOMAKE([dist-bzip2])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([libintl.h])


#find out of C functions gettext dgettext ect. exist
#this will define (or not) HAVE_GETTEXT, HAVE_DGETTEXT, in config.h
AC_CHECK_FUNCS(gettext dgettext dcgettext bindtextdomain textdomain)

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_HEADER_STDBOOL

# Checks for library functions.

# This gets info about gtkmm and puts it into vars
# DEPS_CFLAGS (compile time)
# DEPS_LIBS   (Link time)

PKG_CHECK_MODULES(DEPS, gtkmm-2.4 >= 2.4.0 gconfmm-2.6 >= 2.6.0)
#PKG_CHECK_MODULES(DEPS, gtkmm-2.0 >= 2.2.8)

#make this data visible in Makefile.am
AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)

# give user a chance to tell where boost libraries are located.
# default=/usr
AC_ARG_WITH(boost,
    [ --with-boost=<path> prefix of Boost installation. e.g. /usr/local or /usr],
    [BOOST_PREFIX=$with_boost],
    [BOOST_PREFIX=[/usr]]
)

AC_ARG_WITH(boost_version,
    [ --with-boost_version=1_31 version of Boost installation. e.g. 0_30],
    [BOOST_VERSION=$with_boost_version],
    [BOOST_VERSION=[1_31]]
)

AC_ARG_WITH(boost_include,
    [ --with-boost_version=location of boost include directory],
    [BOOST_INCLUDE=$with_boost_include],
    [BOOST_INCLUDE=${BOOST_PREFIX}/include]
)

AC_ARG_WITH(boost_lib_postfix,
    [ --with-boost_lib_postfix=postfix on boost library files i.e. -gcc-1_31],
    [BOOST_LIB_POSTFIX=$with_boost_lib_postfix],
    [BOOST_LIB_POSTFIX=""]
)


# make BOOST_PREFIX available to Makefile.am
AC_SUBST(BOOST_PREFIX)

# link file, inclued files needed for boost
BOOST_LIBS="-L${BOOST_PREFIX}/lib -lboost_filesystem${BOOST_LIB_POSTFIX} -lboost_regex${BOOST_LIB_POSTFIX}"
BOOST_CXXFLAGS="-I${BOOST_INCLUDE}"

# make data available to Makefile.am
AC_SUBST(BOOST_LIBS)
AC_SUBST(BOOST_CXXFLAGS)

# require a makefile
AC_OUTPUT(Makefile)
