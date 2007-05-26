//
//     PELESS TABBED TEXT FILE LISTER.
//     Copyright (C) 2004 Paul Elliott
//     
//      This file is part of peless.
// 
//     peless is free software; you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation; either version 2 of the License, or
//     (at your option) any later version.
// 
// 
//     peless is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
// 
// 
//     You should have received a copy of the GNU General Public License
//     along with peless; if not, write to the Free Software
//     Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#ifndef _INTERNAT_
#define _INTERNAT_

// See the file INTERNATIONALIZATION!

// grabbed from glade-- 
// use these macros to indicate strings to be translated.
/*
 * Standard gettext macros.
 */
#ifdef ENABLE_NLS
#  include <libintl.h>
#  undef _
#  define _(String) dgettext (GETTEXT_PACKAGE, String)
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#else
// check to see if these functions already exist before using them.
#ifndef TEXTDOMAIN
#  define textdomain(String) (String)
#endif // TEXTDOMAIN
#ifndef HAVE_GETTEXT
#  define gettext(String) (String)
#endif // HAVE_GETTEXT
#ifndef HAVE_DGETTEXT
#  define dgettext(Domain,Message) (Message)
#endif // HAVE_DGETTEXT
#ifndef HAVE_DCGETTEXT
#  define dcgettext(Domain,Message,Type) (Message)
#endif // HAVE_DCGETTEXT
#ifndef HAVE_BINDTEXTDOMAIN
#  define bindtextdomain(Domain,Directory) (Domain)
#endif // HAVE_DCGETTEXT
#  define _(String) (String)
#  define N_(String) (String)
#endif


#endif // _INTERNAT_
