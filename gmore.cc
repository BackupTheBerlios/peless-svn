#include "config.h"
#include "gmore.hh"
#include <gtkmm/dialog.h>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <exception>

#include <boost/filesystem/path.hpp>

namespace Gmore {


Gmore::Gmore(const std::string pfilename):  
  filename(pfilename),   //filename to load
  Gtk::ScrolledWindow(), // scrolled window holds the textview.
  textview()            // view of the data of the file

{

  {
    using namespace std;
    // create a text buffer to hold the text.
    // managed refcounted pointer no memory leak.
    Glib::RefPtr<Gtk::TextBuffer> load_bufferPtr( Gtk::TextBuffer::create() );
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
	  input.open(filename.c_str() );
	  // if open failure thow exception.
	  if ( ! input )
	    {
	      string msg("unable to open ");
	      msg += filename;
	      msg += " for reading.\n";
	      ios_base::failure fail(msg);
	      filename += " Failed to Open.";
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
	  load_buffer.insert( load_buffer.end() , line );
	};
    } 
    // on catching io errors while doing above.
    // display error dialog.
    catch (ios_base::failure& ex){
      Gtk::MessageDialog error_message(ex.what());
      error_message.set_modal();
      error_message.run();
    };

    // load our loaded buffer into the text view.
    textview.set_buffer(load_bufferPtr);


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


Gmore::~Gmore()
{
};



NoteGmore::NoteGmore(int argc, char *argv[],Gtk::WindowType type):
  Gtk::Window(type),
  notebook()
{
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
  // ?? property_destroy_with_parent().set_value(false);

  // reasonable window size
  set_default_size(640,760);


  // for each passed in argument
  for( int i=0; i < argc ; i++)
    {
      // convert argument to string
      std::string fullfilename=argv[i];
      std::string label;
      // label value is ...
      if ( fullfilename.empty() )
	{
	  // STDIN if empty.
	  label=_("STDIN");
	}
      else
	{
	  // if not empty get boost to parse get leaf end of filename.
	  boost::filesystem::path filepath(fullfilename);
	  label= filepath.leaf();
	};

      // hold a pointer to our page locally while we fool with it.
      std::auto_ptr<Gmore::Gmore>
          local_gmore_holder( new Gmore::Gmore(fullfilename) );
      {
	using namespace SigC;
	Gmore::Gmore & page_to_add( *local_gmore_holder );

	page_to_add.set_external_title( _("Gmore : ") + fullfilename );


#if 0
	// We want to call page_to_add.signal_switch_page ().connect( X )
	// where X must be of type Slot2<void, GtkNotebookPage*, guint > 
	// but  GtkNotebookPage*, guint parameters are not needed.
	// so assume we had Y of type Slot0<void> then we could create
	// X by X=
	//hide<guint,void,GtkNotebookPage*>(
	//           hide<GtkNotebookPage*,void>(Slot0<void>) )
	// i.e.
	//hide<guint,void,GtkNotebookPage*>(
	//           hide<GtkNotebookPage*,void>(Y) )
	//
	// but when the signal happens we want this->set_title(Z)
	// to be called where Z is ustring = value of this->title here
	// so we want Q with type Slot1<void,const ustring&> 
	//to be changed into 
	// the form of Y=Slot0<void> fixing this->title to be passed
	// to this->set_title therefore we must use bind
	//
	// Y type Slot0<void> = bind<const ustring&,void,ustring>(Q,title)
	// remember to disconnect the connection is a destructor.
#endif

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

	notebook.signal_switch_page().connect(connect_slot);
#else
	// this is a more compact version of the above.
	// instead of using 2 intermediate vars, change_title_slot
	// and connect_slot, make it one large expression like apl
	// I beleive this is less readable. (but it works as well)
	notebook.signal_switch_page().
	  connect(
		  SigC::bind<RefGmore,void,GtkNotebookPage*, guint, RefGmore>
		  (slot(*this,&NoteGmore::change_title),
		   page_to_add_ref)
		  );
#endif
      };


      // We append the new page
      // we are using a pointer, but we are making it managed
      // and we are immeadiately adding to container (Notebook)
      // which will take responsibility for deletion.

      // release the local holder as container takes responsibility for it.
      notebook.append_page( *manage( local_gmore_holder.release() ), label);

    };

  // show the notebook
  notebook.show();


  // add notebook to our window.
  add(notebook);

  // show our window.
  show();


};


NoteGmore::~NoteGmore()
{
};

void NoteGmore::change_title( 
			     GtkNotebookPage* raw_page, 
			     guint index,
			     RefGmore sub_window_ref
			     )
{
  Gmore::Gmore& gmore_page = sub_window_ref.get();


  if ( (notebook.get_current_page() ) == (notebook.page_num(gmore_page) ) )
    {
      set_title( gmore_page.get_external_title() );
    };
};



} // namespace Gmore
