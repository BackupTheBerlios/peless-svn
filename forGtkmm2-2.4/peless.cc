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

  // convert argv to argument list sequence to pass to NoteGmore.
  Gmore::NoteGmore::file_list_type arguement_list =
    Glib::sequence<Gmore::NoteGmore::file_list_iterator_type>
    (files,files+filecount);

  // create a note book with one page for every file
  Gmore::NoteGmore note_gmore( arguement_list);

  // run Gtk, X11 main event loop.
  Gtk::Main::run(note_gmore);
    
  return 0;
}
