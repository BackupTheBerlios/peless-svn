#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.51)
 AC_INIT(peless, 1.108)
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

#use boost. by default /usr /usr/local and /opt directories will be searched for boost
AX_BOOST_BASE([1.32.1])
#use the boost filesystem library
AX_BOOST_FILESYSTEM
#use the boost regex library.
AX_BOOST_REGEX


#make this data visible in Makefile.am
AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)



# require a makefile
AC_OUTPUT(Makefile)
