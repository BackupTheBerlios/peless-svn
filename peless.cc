#include "config.h"
#include <gtkmm.h>

#include <algorithm>

#include "gmore.hh"


int main(int argc, char *argv[])
{
  // Let gtk get its parameters;
  Gtk::Main kit(argc, argv);

  // filename parameters
  int filecount = argc - 1;
  char **files = argv + 1;

  // if no parameters then one file stdin
  if ( filecount == 0)
    {
      filecount = 1;
      static char * onenullfile[2] = {"",0};
      files = onenullfile;
    };

  
  Gmore::NoteGmore::file_list_type arguement_list(filecount);

  std::copy<char **,Gmore::NoteGmore::file_list_iterator_type> 
    (files,files+filecount,arguement_list.begin() );

  // create a note book with one page for every file
  Gmore::NoteGmore note_gmore( arguement_list);

  // run Gtk, X11 main event loop.
  Gtk::Main::run(note_gmore);
    
  return 0;
}
