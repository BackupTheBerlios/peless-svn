#ifndef _GMORE_
#define _GMORE_ 1

#include "config.h"

//#include "myhide.h"   // sometimes hide.h cause ambiguity
#include <sigc++/sigc++.h>
#include <gtkmm.h>
#include <gtkmm/label.h>
#include <gtkmm/textview.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/notebook.h>
#include <gtkmm/box.h>

#include <string>

#include <boost/ref.hpp>

//#include <iostream>
//#include <fstream>
//#include <iomanip>



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

namespace Gmore {  // avoid namespace conficts.

  // A Gmore is a scrolled window containing
  // and a textview 
  // loaded with the data from the file.
  // scrollwindow allows text to be scrolled.
  class Gmore : public Gtk::ScrolledWindow
  {  
  private:

    // displays text of file
    class Gtk::TextView textview;

        
    // filename of the data displayed
    std::string filename;

  public:
    // construct from filename.
    Gmore(const std::string filename);
    // destructor
    virtual ~Gmore();

    // property gets filename of file displayed.
    const std::string FileName() const { return filename; };

  private:
    //disallow do not define trivial constructor, conv ctr, assignment
    Gmore();
    Gmore(const Gmore&);
    Gmore& operator=(const Gmore&);

    // property haveing title parent window should use to title this file.
    Glib::ustring external_title;
  public:
    // define property set/get methods.
    const Glib::ustring& get_external_title() const 
    { 
      return external_title; 
    };
    void set_external_title(const Glib::ustring& etitle) 
    { 
      external_title=etitle; 
    };

  };

  // the boost library gives us a reference warper
  // with value sematics!
  typedef boost::reference_wrapper<Gmore> RefGmore;

  // a NoteGmore is a notebook containing any number of
  // Gmores.
  class NoteGmore : public Gtk::Window
  {  

  public:
    // construct from list of fileames.
    // list is not fancy contailer vector ala main(int argc, char* argv[])
    NoteGmore(int argc, char *argv[], 
	      Gtk::WindowType type=Gtk::WINDOW_TOPLEVEL);
    virtual ~NoteGmore();


  private:
    void change_title( 
		      GtkNotebookPage*, 
		      guint,
		      // RefGmore rather than Gmore& becaus
		      // of bug in bind. bound paramenter must be
		      // destructable. i.e. not a reference.
		      // boost::reference_wrapper gives us this.
		      RefGmore
		      );


    //disallow do not define trivial constructor, conv ctr, assignment
    NoteGmore();
    NoteGmore(const NoteGmore&);
    NoteGmore& operator=(const NoteGmore&);

    // this is the notebook containing any number of (Gmore)s
    Gtk::Notebook notebook;


  };

} // namespace Gmore

#endif // _GMORE_
