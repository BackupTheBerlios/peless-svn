#include "search.hh"

// implementation for searching TextViews for regular expressions.

namespace SearchTextView {  // avoid namespace conficts.

  SearchCenter::SearchCenter(Gtk::Window& parent): Gtk::Dialog(
					    _("Regular Expression Search"),
					    parent,
					    true,
					    true
					    )
  {

    add_button(Gtk::StockID("gtk-cancel"), Gtk::RESPONSE_CANCEL );
    add_button(Gtk::StockID("gtk-ok"),   Gtk::RESPONSE_OK );

    // exit dialog.run() on above buttons
    set_response_sensitive(Gtk::RESPONSE_CANCEL);
    set_response_sensitive(Gtk::RESPONSE_OK);
    set_default_response(Gtk::RESPONSE_CANCEL);

    // do not show dialog, make it appear on tasklist when not in use!
    // run() it instead.
    //show();
  };

  void SearchCenter::query_find()
  {
    if( run() == Gtk::RESPONSE_OK)
      {
      };
    hide();

  };



} // namespace SearchTextView


