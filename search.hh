#ifndef _TEXTSEARCH_
#define _TEXTSEARCH_ 1

#include "config.h"
#include <gtkmm/textview.h>
#include <gtkmm/dialog.h>
#include <gtkmm/frame.h>
#include <gtkmm/entry.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/checkbutton.h>


#include <boost/regex.hpp>

#include "internat.hh"

// interface for searching TextViews for regular expressions.

namespace SearchTextView {  // avoid namespace conficts.


class SearchCenter: public Gtk::Dialog
{
public:
  SearchCenter(Gtk::Window& parent);

  void query_find();

private:
  SearchCenter();
  SearchCenter(const SearchCenter&);
  SearchCenter& operator=(const SearchCenter&);

  Gtk::VBox regex_vbox;
  Gtk::HBox top_hbox;
  Gtk::Frame entry_frame;
  Gtk::Entry regex_entry;
  Gtk::Frame type_frame;
  Gtk::VBox checkbox_vbox;
  Gtk::VBox type_vbox;
  Gtk::RadioButton_Helpers::Group type_group;
  Gtk::RadioButton literal_radiobutton,normal_radiobutton,
                   basic_radiobutton,extended_radiobutton;
  Gtk::CheckButton exact_checkbutton,reverse_checkbutton;

};


} // namespace SearchTextView



#endif // _TEXTSEARCH_
