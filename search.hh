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
  bool operator==(const NewIter& other)
  {
    return ( base == other.base );
  };

  bool operator!=(const NewIter& other)
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

  //class SearchTextView::NewIter<wchar_t,Gtk::TextIter> TextIterWchar;


class SearchCenter : public   boost::basic_regex<wchar_t>
{
public:


  typedef Glib::ustring::value_type value_type;

  SearchCenter(Gtk::Window& parent);

  void query_find();

  bool forward();

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


#if 0

  // iterator works like TextIter except operator*() is wchar_t

class TextIterWchar
{
public:
  typedef Gtk::TextIter::iterator_category iterator_category;

  // value type is changed!
  typedef wchar_t                         value_type;
  typedef Gtk::TextIter::difference_type  difference_type;
  typedef value_type                      reference;
  typedef Gtk::TextIter::pointer          pointer;

  // can create one from an TextIter
  TextIterWchar(const Gtk::TextIter& existing):
    base(existing)
  {
  };


  // convert back to TextIter by returning base!
  operator Gtk::TextIter&()
  {
    return base;
  };

  // can assign to TextIter by assigning to base
  Gtk::TextIter& operator=(Gtk::TextIter& to)
  {
    to=base;
    return to;
  };

  // value operator by static cast of value of base!
  value_type operator*() 
  { 
    return static_cast<value_type>( base.operator*() ); 
  };

  // do transversal operators by refering to base!
  TextIterWchar& operator++() { base.operator++(); return *this; };
  TextIterWchar const operator++(int i) 
  {
    const TextIterWchar nv( *this );
    base.operator++(i);
    return nv;
  };

  TextIterWchar& operator--() { base.operator--(); return *this; };
  TextIterWchar const operator--(int i) 
  {
    const TextIterWchar nv ( *this );
    base.operator--(i);
    return nv;
  };

private:
  Gtk::TextIter base;

};

#endif

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
private:
  //disallow do not define trivial constructor, conv ctr, assignment
  SearchDisplay();
  SearchDisplay(const SearchDisplay&);
  SearchDisplay& operator=(const SearchDisplay&);

  Gtk::TextView& view;
  Glib::RefPtr<Gtk::TextBuffer> view_buffer;


  Glib::RefPtr<Gtk::TextBuffer::Tag>& found_tag;
};



} // namespace SearchTextView



#endif // _TEXTSEARCH_
