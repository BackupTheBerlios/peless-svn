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
