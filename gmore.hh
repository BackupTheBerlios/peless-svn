#ifndef _GMORE_
#define _GMORE_ 1

#include "config.h"

#include "search.hh"

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
  // a NoteGmore is a notebook containing any number of
  // Gmores.
  class NoteGmore : public Gtk::Window
  {  
  private:
    // A Gmore is a scrolled window containing
    // and a textview 
    // loaded with the data from the file.
    // scrollwindow allows text to be scrolled.
    // subclass to NewsGmore later.
    class Gmore : public Gtk::ScrolledWindow
    {  
    public:
      friend class NoteGmore;
      // displays text of file
      class Gtk::TextView textview;

    private:
      // filename of the data displayed
      std::string filename;

      // reference to owning gmore.
      NoteGmore& the_note_gmore;

    public:
      // construct from filename.
      Gmore(NoteGmore& gmore,
	    const std::string filename,
	    Glib::ustring& font_name);
      // destructor
      virtual ~Gmore();

      // property gets filename of file displayed.
      const std::string FileName() const { return filename; };

      // set font in use
      void set_font_in_use(const Glib::ustring&);

      // hold connections that needs to be disconnected
      // when this page goes away.
      SigC::Connection change_page_connection;
      SigC::Connection change_page_font_connection;

      // create a TextBuffer from the data in our file.
      Glib::RefPtr<Gtk::TextBuffer> load_textbuffer_from_file();
    
      // setup a textview.
      void setup_textview(Glib::RefPtr<Gtk::TextBuffer>,
			  const Glib::ustring& font_name);

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

    friend class NoteGmore::Gmore;
    // the boost library gives us a reference warper
    // with value sematics!
    typedef boost::reference_wrapper<Gmore> RefGmore;


  public:

    typedef char ** file_list_iterator_type;
    typedef Glib::Sequence<NoteGmore::file_list_iterator_type> 
    file_list_type;

    typedef const file_list_iterator_type file_list_const_iterator_type;

    // construct from list of fileames.
    NoteGmore( const file_list_type & ,
	       Gtk::WindowType type=Gtk::WINDOW_TOPLEVEL);
    virtual ~NoteGmore();


  private:
    // set the title to the external title of the current page.
    void change_title( 
		      GtkNotebookPage*, 
		      guint,
		      // RefGmore rather than Gmore& becaus
		      // of bug in bind. bound paramenter must be
		      // destructable. i.e. not a reference.
		      // boost::reference_wrapper gives us this.
		      RefGmore
		      );

    // create a Gmore and add it to the notebook as a page.
    void add_less_page( const std::string& );

    // get filename from user and add coresponding Gmore to notebook
    void OpenNewPage();

    // remove the current page.
    void UnLoadCurrentPage();

    // get new font from user apply to all pages.
    void change_font();
    void change_page_font(RefGmore);
    SigC::Signal0<void> change_page_font_signal;

    //disallow do not define trivial constructor, conv ctr, assignment
    NoteGmore();
    NoteGmore(const NoteGmore&);
    NoteGmore& operator=(const NoteGmore&);

    //pointer to current Gmore; no deletion responsibility
    NoteGmore::Gmore * current_gmore;

    // verticle has menu and display
    Gtk::VBox m_Box;

    Gtk::MenuBar m_MenuBar;
    Gtk::Menu m_Menu_File, m_Menu_Edit;

    // this is the notebook containing any number of (Gmore)s
    Gtk::Notebook notebook;

    //font to use for all the textviews
    Glib::ustring textview_font_name;

  };

} // namespace Gmore

#endif // _GMORE_
