#ifndef _INTERNAT_
#define _INTERNAT_


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
