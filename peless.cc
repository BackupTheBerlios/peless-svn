#include "config.h"
#include <gtkmm.h>

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

  // create a note book with one page for every file
  Gmore::NoteGmore note_gmore( filecount, files );

  // run Gtk, X11 main event loop.
  Gtk::Main::run(note_gmore);
    
  return 0;
}
