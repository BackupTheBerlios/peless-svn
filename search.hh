#ifndef _TEXTSEARCH_
#define _TEXTSEARCH_ 1

#include "config.h"
#include <gtkmm/textview.h>
#include <gtkmm/dialog.h>


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

};


} // namespace SearchTextView



#endif // _TEXTSEARCH_
