#include "config.h"
#include "gmore.hh"
#include <gtkmm/dialog.h>

#include "inittime.hh"

#include <algorithm>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <exception>


#include <boost/filesystem/path.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/bind.hpp>

// collor for regex found tags.
const static char RegexFoundCollor[]="yellow";

class ObjectHolder  // prevent Object from being deleted while I live!
{
private:
  Glib::Object * P;
public:
  ObjectHolder(Glib::Object * O): P(O)
  {
    O->reference();  // increase refcount when constructed
  };
  ~ObjectHolder()
  {
    P->unreference(); // decrease refcount when destroyed
  };
private:
  //disable do not define.
  ObjectHolder();
  ObjectHolder(const ObjectHolder&);
  ObjectHolder& operator=(const ObjectHolder&);
};

namespace Gmore {

  // constructor create from a file, font name
NoteGmore::Gmore::Gmore(
			NoteGmore& gmore,
			const std::string pfilename,
			Glib::ustring& font_name
			):  
  the_note_gmore(gmore), // reference to owning gmore
  filename(pfilename),   //filename to load
  Gtk::ScrolledWindow(), // scrolled window holds the textview.
  textview(),            // view of the data of the file

  // initialize, create the textbuffer pointer
  load_bufferPtr( Gtk::TextBuffer::create(the_note_gmore.tag_table) ),

  // construct textview regex searcher
  search_display(textview,load_bufferPtr,the_note_gmore.regex_found_tag)

{
  // readin textbuffer, and add textview after setup.
  setup_textview( load_textbuffer_from_file(), font_name );

};


NoteGmore::Gmore::~Gmore()
{
  // disconnect connections that uses this page.
  change_page_connection.disconnect();
  change_page_font_connection.disconnect();
};

// return a TextBuffer RefPtr after loading it from the file.
Glib::RefPtr<Gtk::TextBuffer> NoteGmore::Gmore::load_textbuffer_from_file()
{
    using namespace std;
    // create a text buffer to hold the text.
    // managed refcounted pointer no memory leak.
    // use the global tag_table on all created buffers

    // get reference to underlying object
    // RefPtr does not have * operator so this code looks wierd.
    // use explicit call to operator-> instead!
    Gtk::TextBuffer& load_buffer= *( load_bufferPtr.operator->() );

    try {
      // we want to work with a ifstream or cin (if empty filename)
      ifstream input;

      // no deletion responsibility, will point to localobj
      // after we point this to either cin of input
      // we will dereference it creating a reference to file
      // we want to work with.
      istream * inputPtr;
      if ( filename.empty() || ( filename == "-" ) )
	{
	  filename= _("-stdin-");
	  inputPtr= &cin;
	}
      else   // not empty filename we must open.
	{
	  inputPtr=&input;
	  input.open( filename.c_str() );
	  // if open failure thow exception.
	  if ( ! input )
	    {
	      string msg( _("unable to open ") );
	      msg += filename;
	      msg += _(" for reading.\n");
	      ios_base::failure fail(msg);
	      filename += _(" Failed to Open.");
	      throw fail;
	    };
	};

      // in will be a reference to our file input or to cin
      // depending on whether filename was empty.
      istream& in=*inputPtr;

      // in io errors will throw exceptions.
      in.exceptions(ios_base::badbit);

      // read till end of file.
      while( ! in.eof() )
	{
	  string line;
	  // readin line
	  getline(in,line);
	  // add back eol char getline stripped.
	  line += '\n';
	  // add line to our buffer
	  load_buffer.insert( load_buffer.end() , Glib::locale_to_utf8(line) );
	};
    } 
    // on catching io errors while doing above.
    // display error dialog.
    catch (ios_base::failure& ex){
      Gtk::MessageDialog error_message(ex.what());
      error_message.set_modal();
      error_message.run();
    };
    // put the cursor at beginning of buffer.
    load_buffer.place_cursor( load_buffer.begin() );
    return load_bufferPtr;
};

// setup a textview in the case of a already loeaded buffer.
void NoteGmore::Gmore::setup_textview(Glib::RefPtr<Gtk::TextBuffer> buffer,
			   const Glib::ustring& font_name )
{
  {

    // set the font to use.
    set_font_in_use(font_name);

    // load our loaded buffer into the text view.
    textview.set_buffer( buffer );


  };

  // copied from glade--
  textview.set_flags(Gtk::CAN_FOCUS);
  textview.set_editable(false);
  textview.set_cursor_visible(true);
  textview.set_pixels_above_lines(0);
  textview.set_pixels_below_lines(0);
  textview.set_pixels_inside_wrap(0);
  textview.set_left_margin(0);
  textview.set_right_margin(0);
  textview.set_indent(0);
  textview.set_wrap_mode(Gtk::WRAP_NONE);
  textview.set_justification(Gtk::JUSTIFY_LEFT);
  set_flags(Gtk::CAN_FOCUS);
  set_shadow_type(Gtk::SHADOW_NONE);
  set_policy(Gtk::POLICY_ALWAYS, Gtk::POLICY_ALWAYS);
  property_window_placement().set_value(Gtk::CORNER_TOP_LEFT);

  add(textview);

  textview.show();

  show();

};

// set the font to use
void NoteGmore::Gmore::set_font_in_use(const Glib::ustring& our_font)
{

  // if the font is empty then do nothing
  if (our_font.empty() ) return;
  
  // create a font description from our font
  Pango::FontDescription font_description( our_font ) ;

  // get the style
  Glib::RefPtr<Gtk::Style> style = textview.get_style();

  // set the font description into the style
  style->set_font( font_description );

  // set the updated style backinto the textinfo.
  textview.set_style(style);

};

// construct a notebook gmore.
NoteGmore::NoteGmore(
		     const file_list_type& file_list,
		     Gtk::WindowType type
		     ):
  current_gmore(0),
  Gtk::Window(type),
  m_Box(false,0),
  notebook(),
  search_center(*this),
  regex_found_tag(Gtk::TextTag::create("regex found tag") ),
  tag_table( Gtk::TextTagTable::create() ),
  font_name_key("/FontNameKey"),         // key for lookup.
  height_key("/SizeHeight"),             // keys for window size
  width_key("/SizeWidth")
{

  // hold result of lookup
  Glib::ustring font_name_value;
  // use this if lookup fails.
  Glib::ustring font_name_default("Nimbus Mono L, 12");
  // only call init one time, no matter how many NoteGmore's are created.
  // init code called only once.
  static InitTime::FirstTime gconf2_first;
  if ( gconf2_first() )
    {
      Gnome::Conf::init();
    };
  // client to get/save data.
  m_refClient = Gnome::Conf::Client::get_default_client();
  // place to look for answer.
  m_refClient->add_dir("/apps/peless");
  // try to read the font.
  try
    {
      font_name_value = m_refClient->get_string(font_name_key);
    }
  catch ( Gnome::Conf::Error& error)
    {
      font_name_value = font_name_default;
    };
  if ( font_name_value.empty() ) font_name_value = font_name_default;
  // save the font to use.
  textview_font_name = font_name_value;

  int height,width;
  int default_height = 760 ;
  int default_width = 640;

  try
    {
      height = m_refClient->get_int(height_key);
      width  = m_refClient->get_int(width_key);
    }
  catch ( Gnome::Conf::Error& error)
    {
      height = default_height;
      width  = default_width;
    };
  if ( ( height == 0 ) || ( width == 0 ) )
    {
      height = default_height;
      width  = default_width;
    };


  // setup regex_found_tag collor
  regex_found_tag->property_background() = RegexFoundCollor;

  // add the collor tag to tag table
  tag_table->add(regex_found_tag);

  // copied from glade --
  notebook.set_flags(Gtk::CAN_FOCUS);
  notebook.set_show_tabs(true);
  notebook.set_show_border(true);
  notebook.set_tab_pos(Gtk::POS_TOP);
  notebook.set_scrollable(true);              // changed from glade

  // code grabed from glade--
  set_title(_("Gmore"));
  set_modal(false);
  property_window_position().set_value(Gtk::WIN_POS_NONE);
  set_resizable(true);

  // reasonable window size
  set_default_size(width,height);


  // for each passed in argument
  // call add_less_page

#if 1
  // use bind library to create a functor object
  // binding in pointer to member to add_less_page and this
  // giveing a functor of 1 variable the same as member function
  // add_less_page = const string&
  // then use std::for_each to call this functor for all members
  // of the <vector> file_list
  std::for_each
    (file_list.begin(),file_list.end(),
     boost::bind(&NoteGmore::add_less_page,this,  _1) );

#else
  // this is the old way to do it with a traditional for loop.

  for(file_list_const_iterator_type  it=file_list.begin();
       it < file_list.end() ; 
       it++)
    {
      add_less_page(*it);
    };
#endif

  Gtk::Menu::MenuList& file_menulist = m_Menu_File.items();

  file_menulist.push_back( Gtk::Menu_Helpers::MenuElem(
      _("_Open"),  
      Gtk::Menu::AccelKey(_("<control>o")),
      SigC::slot(*this, &NoteGmore::OpenNewPage) ) 
			   );

  file_menulist.push_back( Gtk::Menu_Helpers::MenuElem(
      _("_Close current page"),  
      Gtk::Menu::AccelKey(_("<control>c")),
      SigC::slot(*this, &NoteGmore::UnLoadCurrentPage) ) 
			   );


  file_menulist.push_back( Gtk::Menu_Helpers::MenuElem(
      _("_Quit"),  
      Gtk::Menu::AccelKey(_("<control>q")),
      SigC::slot(*(Gtk::Window*)this, &Window::hide) ) 
			   );

  Gtk::Menu::MenuList& edit_menulist = m_Menu_Edit.items();

  edit_menulist.push_back( Gtk::Menu_Helpers::MenuElem(
      _("Fon_t"),  
      Gtk::Menu::AccelKey(_("<control>t")),
      SigC::slot(*this, &NoteGmore::change_font) ) 
			   );

  //menu items for finding text in buffer
  edit_menulist.push_back( Gtk::Menu_Helpers::MenuElem(
      _("_Find"),  
      Gtk::Menu::AccelKey(_("<control>f")),
      SigC::slot(*this, &NoteGmore::find) ) 
			   );

  edit_menulist.push_back( Gtk::Menu_Helpers::MenuElem(
      _("Find _Next"),  
      Gtk::Menu::AccelKey(_("<control>n")),
      SigC::slot(*this, &NoteGmore::find_next) ) 
			   );


  //Add the menus to the MenuBar:
  m_MenuBar.items().push_back( 
	Gtk::Menu_Helpers::MenuElem(_("_File"), m_Menu_File) );
  m_MenuBar.items().push_back( 
        Gtk::Menu_Helpers::MenuElem(_("_Edit"), m_Menu_Edit) );

  //Add the MenuBar to the window:
  m_Box.pack_start(m_MenuBar, Gtk::PACK_SHRINK);
  // add notebook to our window.
  m_Box.pack_start(notebook);

  // show the menubar.
  m_MenuBar.show();

  // show the notebook
  notebook.show();

  // show our VBox.
  m_Box.show();

  add(m_Box);
  show();

};

// destroy the notebook gmore
NoteGmore::~NoteGmore()
{
  int width,height;
  get_size(width,height);
  try
    {
      // store the new font name in gconf
      m_refClient->set(width_key,width);
      m_refClient->set(height_key,height);
    }
  catch( Gnome::Conf::Error& error )
    {
      // problem saving font
      Glib::ustring text(_("error saving window size in gconf\n"));
      text += error.what();
      text += '\n';
      Gtk::MessageDialog msg(
			     text,
			     Gtk::MESSAGE_ERROR,
			     Gtk::BUTTONS_OK,
			     true
			     );

    };

};

// add a page to the notebook
void NoteGmore::add_less_page(const std::string& fullfilename)
{

  //determine file to use.
  std::string label;
  // label value is ...
  if ( fullfilename.empty() )
    {
      // STDIN if empty.
      label=_("STDIN");
    }
  else
    {
      try
	{
	  // if not empty get boost to parse get leaf end of filename.
	  boost::filesystem::path filepath(fullfilename);
	  label= filepath.leaf();
	}
      catch(boost::filesystem::filesystem_error& error)
	{
	  using namespace std;
	  cerr << "problem=" << error.what() << endl;
	  label=fullfilename;
	};
    };

  // construct our page!
  // hold a pointer to our page locally while we fool with it.
  std::auto_ptr<Gmore::Gmore>
    local_gmore_holder( new 
			Gmore::Gmore(
				     *this,
				     fullfilename , 
				     textview_font_name) 
			);
  {
    using namespace SigC;
    Gmore::Gmore & page_to_add( *local_gmore_holder );

    //    page_to_add.set_font_in_use(textview_font_name);

    page_to_add.set_external_title( _("Gmore : ") + fullfilename );



    RefGmore page_to_add_ref(page_to_add);

#if 1
    // when the signal happens we want our method,
    // change_title to be called, it returns void
    // and takes 3 parameters:
    //		      GtkNotebookPage*, 
    //		      guint,
    //		      RefGmore
    // Create a slot for this!

    const Slot3<void,GtkNotebookPage*, guint, RefGmore> change_title_slot =
      slot(*this,&NoteGmore::change_title);
	
    // But the slot above is not appropriate to be connected
    // to signal_switch_page! signal_switch_page
    // wants only 2 parameters:
    //		      GtkNotebookPage*, 
    //		      guint,
    // The extra parameter
    // RefGmore i.e. page_to_add, we want to add to the call
    // We "bind" in the extra parameter with a call to SigC::bind
    // it converts slots by "binding" in an extra parameter.
    // The extra parameter must have "value" semantics
    // so we can not use type Gmore& reference to Gmore
    // because when the parameter to be bound in is reference
    // bind has the reference to reference problem
    // we can not use type Gmore, because Gmore is derived
    // from a window and therefore does not have a copy constructor or
    // assignment operator i.e. value semantics.
    // fortunately the boost library has a solution to this
    // template reference warper
    //  typedef boost::reference_wrapper<Gmore> RefGmore;
    // reference wrappers can be constructed from a type
    // implement reference functionality, and have
    // value sematics.


    const Slot2<void,GtkNotebookPage*, guint> connect_slot =
      SigC::bind<RefGmore,void,GtkNotebookPage*, guint, RefGmore>
      (change_title_slot,page_to_add_ref);

    // now we have an appropriate slot connect it!
    page_to_add.change_page_connection =
    notebook.signal_switch_page().connect(connect_slot);

#else
    // this is a more compact version of the above.
    // instead of using 2 intermediate vars, change_title_slot
    // and connect_slot, make it one large expression like apl
    // I beleive this is less readable. (but it works as well)
    page_to_add.change_page_connection =
      notebook.signal_switch_page().
      connect(
	      SigC::bind<RefGmore,void,GtkNotebookPage*, guint, RefGmore>
	      (slot(*this,&NoteGmore::change_title),
	       page_to_add_ref)
	      );
#endif

    page_to_add.change_page_font_connection =
      change_page_font_signal.
      connect(

	      SigC::bind<RefGmore,void, RefGmore>
	      (slot(*this,&NoteGmore::change_page_font),
	       page_to_add_ref)


             );
  };


  // We append the new page
  // we are using a pointer, but we are making it managed
  // and we are immeadiately adding to container (Notebook)
  // which will take responsibility for deletion.

  // release the local holder as container takes responsibility for it.
  notebook.append_page( *manage( local_gmore_holder.release() ), label);
};

// if this sub_window_ref is the current page
// the change the title of main window.
void NoteGmore::change_title( 
			     GtkNotebookPage* raw_page, 
			     guint index,
			     RefGmore sub_window_ref
			     )
{
  Gmore::Gmore& gmore_page = sub_window_ref.get();

  // if the page given to us in the current page....
  if ( (notebook.get_current_page() ) == (notebook.page_num(gmore_page) ) )
    {
      // set the title of the main page.
      set_title( gmore_page.get_external_title() );

      // keep track of which gmore is the current one.
      current_gmore = & gmore_page;
    };
};


// code to open and load a new page.
void NoteGmore::OpenNewPage()
{

  // setup open file dialog.
  Gtk::FileSelection selection( _("Select File to display") );
  selection.set_select_multiple(true);
  selection.hide_fileop_buttons();
  // run dialog , if not ok, exit.
  if (selection.run() != Gtk::RESPONSE_OK ) return;

  // get selected files
  Glib::ArrayHandle<std::string> selections=selection.get_selections();
  Glib::ArrayHandle<std::string>::iterator it(selections.begin() );
  // for each selected file
  for( it=selections.begin(); it<selections.end(); it++)
    {

      // add page to notebook.
      add_less_page( *it );
    };

};
// code to remove current page.
void NoteGmore::UnLoadCurrentPage()
{
  // get current page
  int current = notebook.get_current_page();


  if (current >= 0 ) 
    {
      // remove it.
      notebook.remove_page(current);
    };
};

// get new font from user, change.
void NoteGmore::change_font()
{
  // setup font seelection dialog.
  Gtk::FontSelectionDialog dialog;
  // run dialog if not OK done.
  if (dialog.run() != Gtk::RESPONSE_OK ) return;
  // get the font selection.
  Gtk::FontSelection& get_selection = *dialog.get_font_selection();

  // get the font name from the font selection.
  textview_font_name = get_selection.get_font_name();
  try
    {
      // store the new font name in gconf
      m_refClient->set(font_name_key,textview_font_name);
    }
  catch( Gnome::Conf::Error& error )
    {
      // problem saving font
      Glib::ustring text(_("error saving fontname in gconf\n"));
      text += error.what();
      text += '\n';
      Gtk::MessageDialog msg(
			     text,
			     Gtk::MESSAGE_ERROR,
			     Gtk::BUTTONS_OK,
			     true
			     );

    };

  // save current page..
  int start_current = notebook.get_current_page();
#if 0
  for( int j=0; j <2;j++)
  for(int i=0; i < notebook.get_n_pages(); i++)
    {
      // get current page as a widget
      Gtk::Widget * current_widgetP = notebook.get_nth_page(i);


      // WARNING WARNING WARNING
      // DOWNCAST, detect errors at runtime.
      // replace this code.
      Gmore::Gmore * gmoreP = dynamic_cast<Gmore::Gmore*>(current_widgetP);
      if (gmoreP)
	{
	  //create reference to the gmore.
	  Gmore::Gmore& current_gmore = *gmoreP;

	  // get the label assigned.
	  Glib::ustring label=notebook.get_tab_label_text(current_gmore);

	  // hide the text view
	  current_gmore.textview.hide();
	  // set current page to us
	  notebook.set_current_page(i);
	  // remove the textview from gmore
	  current_gmore.remove();

	  // keep gmore from being deleted.
	  ObjectHolder hold(gmoreP);
	  //don't work:
	  //Glib::RefPtr<Gmore::Gmore> hold(gmoreP);
	  //SigC::Ptr<Gmore::Gmore> hold ( manage(gmoreP) );

	  // remove from notebook (will not be deleted)
	  notebook.remove_page(current_gmore);

	  // new font in use
	  current_gmore.set_font_in_use(textview_font_name);

	  // show text view add back to gmore.
	  current_gmore.textview.show();
	  current_gmore.add(current_gmore.textview);

	  // put the gmore back into the norebook
	  notebook.insert_page(current_gmore,label,i);

	};
    };
#else
  change_page_font_signal.emit();
#endif
  // restore current page.
  notebook.set_current_page(start_current);


};
void NoteGmore::change_page_font(RefGmore sub_window_ref)
{
  Gmore::Gmore& gmore_page = sub_window_ref.get();

  int i = notebook.page_num(gmore_page);

  // get the label assigned.
  Glib::ustring label=notebook.get_tab_label_text(gmore_page);

  // hide the text view
  gmore_page.textview.hide();
  // set current page to us
  notebook.set_current_page(i);
  // remove the textview from gmore
  gmore_page.remove();

  // keep gmore from being deleted.
  ObjectHolder hold(&gmore_page);
  //don't work:
  //Glib::RefPtr<Gmore::Gmore> hold(&gmore_page);
  //SigC::Ptr<Gmore::Gmore> hold ( manage(&gmore_page) );

  // remove from notebook (will not be deleted)
  notebook.remove_page(gmore_page);

  // new font in use
  gmore_page.set_font_in_use(textview_font_name);

  // show text view add back to gmore.
  gmore_page.textview.show();
  gmore_page.add(gmore_page.textview);

  // put the gmore back into the norebook
  notebook.insert_page(gmore_page,label,i);

};

// find string in buffer after getting string using dialog.
void NoteGmore::find()
{
  //if there is 1 buffer (current)
  if( current_gmore )
    {
      search_center.query_find();
      find_next();
    };
};
// find next occurance of string in buffer
void NoteGmore::find_next()
{
  //if there is 1 buffer (current)
  if( current_gmore )
    {
      // search for the regex and scroll to it.
      current_gmore->search_display.SearchAndScroll(search_center);
    };
};

} // namespace Gmore
