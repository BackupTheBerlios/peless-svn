#ifndef _TEXTSEARCH_
#define _TEXTSEARCH_ 1

#include "config.h"
#include <gtkmm/textview.h>
#include <gtkmm/textbuffer.h>
#include <gtkmm/dialog.h>
#include <gtkmm/frame.h>
#include <gtkmm/entry.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/checkbutton.h>

#include <boost/regex.hpp>

#include "internat.hh"


#ifndef  __STDC_ISO_10646__
#error wchar_T must be UCS-4
#endif // __STDC_ISO_10646__

// I am requiring that wchar_t be a UCS-4 that is =gunichar with static-cast


// interface for searching TextViews for regular expressions.

namespace SearchTextView {  // avoid namespace conficts.

  // modify iterator's pointed to type.
  //Requirement: OldIter must be bi-di traversal iterator
  //with equality
  // static_cast<New>(OltIter::value_type) must be OK.
  //Result is bidi traversal iterator with value_type New
  //can be assigned or converted back to OldIter
template<typename New, typename OldIter >
class NewIter
{
public:
  typedef typename OldIter::iterator_category iterator_category;
  typedef New value_type;
  typedef typename OldIter::difference_type difference_type;
  typedef typename OldIter::reference     reference;
  typedef New *pointer;

  // required default constructor
  NewIter(): base() {};
  // can create one from an OldIter
  NewIter(const OldIter& existing):
    base(existing)
  {
  };

  // convert back to OldIter by returning base!
  operator OldIter&()
  {
    return base;
  };

  // can assign to OldIter by assigning to base
  OldIter& operator=(OldIter& to)
  {
    to=base;
    return to;
  };

  // value operator by static cast of value of base!
  value_type operator*() 
  { 
    return static_cast<value_type>( base.operator*() ); 
  };

  // equality operators
  bool operator==(const NewIter& other) const
  {
    return ( base == other.base );
  };

  bool operator!=(const NewIter& other) const
  {
    return ( base != other.base );
  };


  // do transversal operators by refering to base!
  NewIter& operator++() { base.operator++(); return *this; };
  NewIter const operator++(int i) 
  {
    const NewIter nv( *this );
    base.operator++(i);
    return nv;
  };

  NewIter& operator--() { base.operator--(); return *this; };
  NewIter const operator--(int i) 
  {
    const NewIter nv ( *this );
    base.operator--(i);
    return nv;
  };

private:
  OldIter base;

};



class SearchCenter : public   boost::basic_regex<wchar_t>
{
public:


  typedef Glib::ustring::value_type value_type;

  SearchCenter(Gtk::Window& parent);

  void query_find();

  bool search_forward();

  bool Empty() { return regex_string.empty(); };

private:
  SearchCenter();
  SearchCenter(const SearchCenter&);
  SearchCenter& operator=(const SearchCenter&);

  // dialog elements.
  Gtk::Dialog dialog;
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
  //end of dialog elements

  Glib::ustring regex_string;

};



// does search scrool TextView to display results.
class SearchDisplay
{
public:
  // requirement: view uses a TextBuffer that has a TextTable that
  // contains the found_tag!
  SearchDisplay(
		Gtk::TextView& view, 
		Glib::RefPtr<Gtk::TextBuffer> view_buffer,
		Glib::RefPtr<Gtk::TextBuffer::Tag>& found_tag);

  void SearchAndScroll(SearchCenter& search_center);

  typedef SearchTextView::NewIter<wchar_t,Gtk::TextBuffer::iterator> 
                                                     TextBufferIterWchar;

private:
  //disallow do not define trivial constructor, conv ctr, assignment
  SearchDisplay();
  SearchDisplay(const SearchDisplay&);
  SearchDisplay& operator=(const SearchDisplay&);

  Gtk::TextView& view;
  Glib::RefPtr<Gtk::TextBuffer> view_buffer;

  Gtk::TextBuffer& buffer;

  Glib::RefPtr<Gtk::TextBuffer::Tag>& found_tag;


  // limits on regex found string if == then empty.
  TextBufferIterWchar regex_found_begin,regex_found_end;

  // is found buffer empty
  bool RegexFoundEmpty() const 
  { 
    return ( regex_found_begin == regex_found_end );
  };

  // 
  void RegexSetFoundEmpty()
  {
    regex_found_begin = regex_found_end = buffer.begin();
  };


  bool search_region(
		     bool forward,
		     SearchCenter& search_center,
		     SearchDisplay::TextBufferIterWchar reg_begin,
		     SearchDisplay::TextBufferIterWchar reg_end
		     );
  bool display_message_YES(const Glib::ustring& msg);
  void display_message_OK(const Glib::ustring& msg);
};

} // namespace SearchTextView



#endif // _TEXTSEARCH_
