#include <locale>

#include "search.hh"
#include <gtkmm/messagedialog.h>
#include <exception>


// implementation for searching TextViews for regular expressions.


namespace SearchTextView {  // avoid namespace conficts.

  SearchCenter::SearchCenter(Gtk::Window& parent): 
    dialog(
		_("Regular Expression Search"),
		parent,
		true,
		true
		),

    // init shown by glade
    regex_vbox(false,0),
    top_hbox(false,0),
    entry_frame(_("Regular Expression search string")),
    regex_entry(),
    type_frame(_( "Regular Expression type" )),
    checkbox_vbox(false,0),
    type_vbox(false,0),
    type_group(),
    literal_radiobutton(type_group,_("literal")),
    normal_radiobutton(type_group,_("normal")),
    basic_radiobutton(type_group,_("basic")),
    extended_radiobutton(type_group,_("extended")),

    exact_checkbutton(_("require exact case match")),
    reverse_checkbutton(_("reverse search direction")),
    regex_string()
  {
    imbue(std::locale::classic());
    // most of this shown by glade-2.
    dialog.add_button(Gtk::StockID("gtk-cancel"), Gtk::RESPONSE_CANCEL );
    dialog.add_button(Gtk::StockID("gtk-ok"),   Gtk::RESPONSE_OK );

    // exit dialog.run() on above buttons
    dialog.set_response_sensitive(Gtk::RESPONSE_CANCEL);
    dialog.set_response_sensitive(Gtk::RESPONSE_OK);
    dialog.set_default_response(Gtk::RESPONSE_OK);

    dialog.get_vbox()->pack_start(regex_vbox);
    dialog.get_vbox()->set_homogeneous(false);
    dialog.get_vbox()->set_spacing(0);

    regex_vbox.pack_start(top_hbox);
    regex_vbox.pack_start(entry_frame, Gtk::PACK_SHRINK, 0);

    regex_entry.set_flags(Gtk::CAN_FOCUS);
    regex_entry.set_visibility(true);
    regex_entry.set_editable(true);
    regex_entry.set_max_length(0);
    regex_entry.set_text(_(""));
    regex_entry.set_has_frame(true);
    regex_entry.set_activates_default(true);

    entry_frame.set_border_width(3);
    entry_frame.set_shadow_type(Gtk::SHADOW_ETCHED_IN);
    entry_frame.set_label_align(0,0.5);
    entry_frame.add(regex_entry);

    top_hbox.pack_start(type_frame);
    top_hbox.pack_start(checkbox_vbox);

    type_frame.set_shadow_type(Gtk::SHADOW_ETCHED_IN);
    type_frame.set_label_align(0,0.5);
    type_frame.add(type_vbox);

    exact_checkbutton.set_flags(Gtk::CAN_FOCUS);
    exact_checkbutton.set_relief(Gtk::RELIEF_NORMAL);
    exact_checkbutton.set_mode(true);
    exact_checkbutton.set_active(true);
    reverse_checkbutton.set_flags(Gtk::CAN_FOCUS);
    reverse_checkbutton.set_relief(Gtk::RELIEF_NORMAL);
    reverse_checkbutton.set_mode(true);
    reverse_checkbutton.set_active(false);

    checkbox_vbox.pack_start(exact_checkbutton, Gtk::PACK_SHRINK, 0);
    checkbox_vbox.pack_start(reverse_checkbutton, Gtk::PACK_SHRINK, 0);

    literal_radiobutton.set_flags(Gtk::CAN_FOCUS);
    literal_radiobutton.set_relief(Gtk::RELIEF_NORMAL);
    literal_radiobutton.set_mode(true);
    literal_radiobutton.set_active(false);
    normal_radiobutton.set_flags(Gtk::CAN_FOCUS);
    normal_radiobutton.set_relief(Gtk::RELIEF_NORMAL);
    normal_radiobutton.set_mode(true);
    normal_radiobutton.set_active(true);
    basic_radiobutton.set_flags(Gtk::CAN_FOCUS);
    basic_radiobutton.set_relief(Gtk::RELIEF_NORMAL);
    basic_radiobutton.set_mode(true);
    basic_radiobutton.set_active(false);
    extended_radiobutton.set_flags(Gtk::CAN_FOCUS);
    extended_radiobutton.set_relief(Gtk::RELIEF_NORMAL);
    extended_radiobutton.set_mode(true);
    extended_radiobutton.set_active(false);

    type_vbox.pack_start(literal_radiobutton, Gtk::PACK_SHRINK, 0);
    type_vbox.pack_start(normal_radiobutton, Gtk::PACK_SHRINK, 0);
    type_vbox.pack_start(basic_radiobutton, Gtk::PACK_SHRINK, 0);
    type_vbox.pack_start(extended_radiobutton, Gtk::PACK_SHRINK, 0);

    exact_checkbutton.show();
    reverse_checkbutton.show();

    literal_radiobutton.show();
    normal_radiobutton.show();
    basic_radiobutton.show();
    extended_radiobutton.show();

    type_vbox.show();
    type_frame.show();
    checkbox_vbox.show();
    top_hbox.show();
    regex_entry.show();
    entry_frame.show();
    regex_vbox.show();

    // text entry has focus when dialog starts.
    dialog.set_focus(regex_entry);
    // most of the above came from glade-2

    // do not show dialog, make it appear on tasklist when not in use!
    // run() it instead.
    //show();
  };
  typedef NewIter<wchar_t,Glib::ustring::iterator> wchar_ustring_iterator;
  void SearchCenter::query_find()
  {
    if( dialog.run() == Gtk::RESPONSE_OK)
      {

	// get the new regular expression search string.
	regex_string = regex_entry.get_text();

	boost::regex_constants::syntax_option_type flags=0;
	if ( !exact_checkbutton.get_active() )
	  {
	    flags |= boost::regex_constants::icase;
	  };
	if ( literal_radiobutton.get_active() )
	  {
	    flags |= boost::regex_constants::literal;
	  };
	if ( normal_radiobutton.get_active() )
	  {
	    flags |= boost::regex_constants::normal;
	  };
	if ( basic_radiobutton.get_active() )
	  {
	    flags |= boost::regex_constants::basic;
	  };
	if ( extended_radiobutton.get_active() )
	  {
	    flags |= boost::regex_constants::extended;
	  };
	if ( ! regex_string.empty() )
	  try 
	    {
	      assign(
		   wchar_ustring_iterator(regex_string.begin()),
		   wchar_ustring_iterator(regex_string.end()),
		   flags);

	    }
	  catch( std::exception& excpt)
	    {
	      // if exception send message dialog.
	      std::string msg=_("Error compiling regular exception:\n");
	      msg += excpt.what();
	      Gtk::MessageDialog msgdia(
					dialog,              //parent
					msg,                 // message str
					Gtk::MESSAGE_WARNING, // severity
					Gtk::BUTTONS_OK,     // OK button
					true                 // modal
					);
	      msgdia.run();
	      regex_entry.set_text("");
	      regex_string="";
	    };
      };
    dialog.hide();

  };

  bool SearchCenter::search_forward()
  {
    return !reverse_checkbutton.get_active();
  };

  SearchDisplay::SearchDisplay(
		Gtk::TextView& viewA, 
		Glib::RefPtr<Gtk::TextBuffer> view_bufferA,
		Glib::RefPtr<Gtk::TextBuffer::Tag>& found_tagA
		):
    view(viewA),                          //TextView
    view_buffer( view_bufferA ),          //TextBuffer ref pointer
    buffer( *view_buffer.operator->() ),  //TextBuffer convieneince reference
    found_tag(found_tagA),                // tag to use.
    // empty make it point somewhere
    regex_found_begin( buffer.begin() ),
    // but must _begin==_end i.e. empty
    regex_found_end( buffer.begin() )   
  {
  };

  void SearchDisplay::SearchAndScroll(SearchCenter& search_center)
  {
    // get direction.
    bool go_forward = search_center.search_forward();
    if ( ! RegexFoundEmpty() )
      {
	buffer.remove_tag(
			  found_tag,
			  Gtk::TextBuffer::iterator(regex_found_begin),
			  Gtk::TextBuffer::iterator(regex_found_end) 
			  );
      };

    // on empty regular expression return.
    if ( search_center.Empty()   ) 
      {
	if ( ! RegexFoundEmpty() )
	  {
	    // for some strange reason after failure
	    // must remove the old tag a second time! possible bug.
	    buffer.remove_tag(
			      found_tag,
			      Gtk::TextBuffer::iterator(regex_found_begin),
			      Gtk::TextBuffer::iterator(regex_found_end) 
			      );
	  };
	return;
      };
    Gtk::TextBuffer::iterator region_begin,region_end;

    bool success;
    if (go_forward)
      {
	region_begin = buffer.get_iter_at_mark( buffer.get_insert() );
	region_end   = buffer.end();
	success =
	  search_region(go_forward,search_center,region_begin,region_end);
	if ( ! success && 
	     (region_begin != buffer.begin() ) && 
	     display_message_YES( _(
	      "Regular expresion search failed, continue search from "
              "beginning of buffer."
                                ) )
	     )
	  {
	    region_end = region_begin;
	    region_begin = buffer.begin();
	    success =
	      search_region(go_forward,search_center,region_begin,region_end);
	  };
      }
    else
      {
	region_begin   = buffer.begin();
	region_end = buffer.get_iter_at_mark( buffer.get_insert() );
	success =
	  search_region(go_forward,search_center,region_begin,region_end);
	if ( ! success && 
	     (region_end != buffer.end() ) && 
	     display_message_YES( _(
	      "Regular expresion search failed, continue search from "
              "end of buffer."
                                ) )
	     )
	  {
	    region_begin = region_end;
	    region_end = buffer.end();
	    success =
	      search_region(go_forward,search_center,region_begin,region_end);
	  };
      };

    if ( ! success )
      {
	display_message_OK(
			   _("Regular expression search failure.")
                          );
      };

    // if search failure empty found region
    if ( success && ( ! RegexFoundEmpty() ) )
      {
	buffer.apply_tag(
			 found_tag,
			 Gtk::TextBuffer::iterator(regex_found_begin),
			 Gtk::TextBuffer::iterator(regex_found_end) 
			 );
	Gtk::TextBuffer::iterator found_begin = regex_found_begin;
	Gtk::TextBuffer::iterator found_end = regex_found_end;
	view.scroll_to_iter(found_begin,0.0);
	buffer.place_cursor( 
           ( go_forward? regex_found_end: regex_found_begin) 
	   );
      }
    else
      {
	// for some strange reason after failure
	// must remove the old tag a second time! possible bug.
	buffer.remove_tag(
			  found_tag,
			  Gtk::TextBuffer::iterator(regex_found_begin),
			  Gtk::TextBuffer::iterator(regex_found_end) 
			  );
	// now no found string.
	RegexSetFoundEmpty();
      };
  };

  // search a defined region in a direction for regex
  bool SearchDisplay::search_region(
			   bool forward,
			   SearchCenter& search_center,
			   SearchDisplay::TextBufferIterWchar reg_begin,
			   SearchDisplay::TextBufferIterWchar reg_end
			   )
  {
    if (search_center.Empty() ) return false;
    bool success = false;  // assume failure
    // if forward
    if (forward)
      {
	// search for match.
	boost::match_results<SearchDisplay::TextBufferIterWchar> result;
	success = regex_search(reg_begin,reg_end,result,search_center);
	if(success)
	  {
	    // if match store location of match.
	    regex_found_begin = result[0].first;
	    regex_found_end   = result[0].second;
	  };
      }
    else
      {
	//to search in reverse direction
	//search in forward direction untill failure
	while ( search_region(true,search_center,reg_begin,reg_end) )
	  {
	    // if at least one search is success, then total is success.
	    success |= true;
	    // next search starts at end of last match.
	    reg_begin = regex_found_end;
	  };
	// last sucessfull search will find match closest to end.
      };
    return success;
  };
  bool SearchDisplay::display_message_YES(const Glib::ustring& msg)
  {
    //message dialog yes/no
    Gtk::MessageDialog msgdia(
			      msg,                 // message str
			      Gtk::MESSAGE_WARNING, // severity
			      Gtk::BUTTONS_YES_NO,  // OK button
			      true                 // modal
			      );
    return ( msgdia.run() == Gtk::RESPONSE_YES );   //run the dialog

  };
  void SearchDisplay::display_message_OK(const Glib::ustring& msg)
  {
    // message dialog OK
    Gtk::MessageDialog msgdia(
			      msg,                 // message str
			      Gtk::MESSAGE_WARNING,// severity
			      Gtk::BUTTONS_OK,     // OK button
			      true                 // modal
			      );
      msgdia.run();

  };


} // namespace SearchTextView


