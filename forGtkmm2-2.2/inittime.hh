//
//     PELESS TABBED TEXT FILE LISTER.
//     Copyright (C) 2004 Paul Elliott
//     
//     This program is free software; you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation; version 1.
//     
//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
//     
//     You should have received a copy of the GNU General Public License
//     along with this program; if not, write to the Free Software
//     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
//     Paul Elliott pelliott@io.com
#ifndef _INITTIME_
#define _INITTIME_ 1
#include  <boost/utility.hpp>

// Am I reinventing the wheel here? If there is a publicly released
// class that does the same thing, email pelliott@io.com.

namespace InitTime {

  // functional object returns true first time
  // false thereafter.
  // Use:
  // Declare one of these static scope, then
  // use as test to call initialization code.
  // static InitTime::FirstTime my_first;
  //  ...code
  // if (my_first() ) my_init();
  //

  class FirstTime: boost::noncopyable
  {
  public:
    FirstTime():
      not_first(true)   // initial value true: will return false first time.
    {
    };
    // firsttime true, false forevermore.
    bool operator()()
    {
      bool result = not_first;   // first call?
      not_first = false;         // false forevermore.
      return result;             // was this the first
    };
  private:
    bool not_first;
  };

};

#endif // _INITTIME_
