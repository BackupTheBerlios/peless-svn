#include "search.hh"

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

    // most of the above came from glade-2

    // do not show dialog, make it appear on tasklist when not in use!
    // run() it instead.
    //show();
  };

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

	assign(
	       regex_string.begin(),
	       regex_string.end(),
	       flags);

      };
    dialog.hide();

  };

  bool SearchCenter::forward()
  {
    return !reverse_checkbutton.get_active();
  };



} // namespace SearchTextView


