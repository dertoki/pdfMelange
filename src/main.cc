/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * This file is part of pdfMelange.
 * Copyright (C) Tobias Tandetzki 2014 <tobzki@googlemail.com>
 *
 * PDFmelange is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * pdfMelange is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "melangeDrawingArea.h"
#include "melangeWindow.h"
#include "melangeKeyFile.h"
#include <gtkmm.h>
#include <iostream>
#include <libintl.h>

#ifdef __WIN32
    #define GETTEXT_PACKAGE "pdfmelange"
    #define PROGRAMNAME_LOCALEDIR "../share/locale"
#else
    #include "config.h"
#endif

Glib::ustring get_file_uri(Glib::ustring file_string);

/**
 *  \brief Entry point of the pdfMelange.
 *
 *  Workflow:
 *  1. bind to locale language.
 *  2. load ui builder file or ustring header to build the GUI.
 *  3. instantiate a gtk main window.
 *  4. start gtk main loop.
 *  5. clean up.
 *
 * \param argc: argument count, number of arguments on the command line.
 * \param *argv[]: argument vector, array of null-terminated strings,
 *                 representing the arguments that were entered on the command line
 *                 when the program was started.
 * \return -
 */
int main (int argc, char *argv[])
{
    gchar *locale;

    /** bind locales */
    bindtextdomain(GETTEXT_PACKAGE, PROGRAMNAME_LOCALEDIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);
    locale = setlocale (LC_ALL, "");

    Gtk::Main kit(argc, argv);

    //check if argv[1] is a valid filename
    Glib::ustring file_uri;
    if (argc != 2) 
		file_uri = "";
	else
        file_uri = get_file_uri(argv[1]);
	
    //Load the Glade file and initiate its widgets:
    Glib::RefPtr<Gtk::Builder> builder;
    try
    {
        //builder = Gtk::Builder::create_from_file(UI_FILE);
        /** to create the ui-string header call "gtkbuilder2ustring ../src/melange.ui ../src/melange.ui-string.h" */
        #include "melange.ui-string.h"
        builder = Gtk::Builder::create_from_string(ui_builder);
    }
    catch (const Glib::FileError & ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    // create the main instance
    melangeWindow* pMelangeWindow = NULL;
    builder->get_widget_derived("main_window", pMelangeWindow);

    // apply some settings to the main instance
    pMelangeWindow->set_programFileName(argv[0]);

	/** Bind the application icon */
	// How the inline csource has been created:
    // $ gdk-pixbuf-csource --raw  --name=pixbuf_inline pixmaps/pdfMelange.png > src/melange_pixbuf-inline.h
    #include"melange_pixbuf-inline.h"
    Glib::RefPtr<Gdk::Pixbuf> melangeIcon;
    melangeIcon = Gdk::Pixbuf::create_from_inline( 4096, pixbuf_inline);
    pMelangeWindow->set_icon(melangeIcon);
    pMelangeWindow->set_default_icon(melangeIcon);

    if (!file_uri.empty())
    {
        pMelangeWindow->set_mainFileName(Glib::filename_from_uri(file_uri));
        pMelangeWindow->append_document(file_uri);
    }
    else
    {
        pMelangeWindow->set_mainFile_is_modified(false);
    }


    // start the main loop
    if (pMelangeWindow)
    {
        kit.run(*pMelangeWindow);
    }

    // clean up
    delete pMelangeWindow;
    g_free ( locale );

    return 0;
}

/**
 *  \brief Check if the given argment is a valid filename or url.
 * 
 * If filename is not a full filename, it tries to create the full filename and uri.
 *
 *  \param Glib::ustring file_string: filename or uri.
 *  \return uri if file exists, otherwise an empty string.
 */
Glib::ustring get_file_uri(Glib::ustring file_string)
{
    std::string uri;
    std::cout << "Input given: " << file_string << std::endl;

	// check if uri was given.
	try
	{
        uri = file_string;
		if(Glib::file_test(Glib::filename_from_uri(uri), Glib::FILE_TEST_EXISTS))
		{
		    std::cout << "Trying to open: " << uri << std::endl;
			return uri;
		}
	}
    catch (const Glib::ConvertError &ex)
	{
		std::cout << ex.what() << std::endl;
	}
	
	// check if filename was given.
	try
    {
        uri = Glib::filename_to_uri (file_string);
		if(Glib::file_test(Glib::filename_from_uri(uri), Glib::FILE_TEST_EXISTS))
		{
		    std::cout << "Trying to open: " << uri << std::endl;
			return uri;
		}
    }
    catch (const Glib::ConvertError &ex)
    {
		std::cout << ex.what() << std::endl;
    }

	// try to create a valid uri
	Glib::ustring absolut_name = Glib::build_filename(Glib::get_current_dir(), file_string);
    std::cout << "Created absolut filename:" <<  absolut_name << std::endl;
	try
	{
		uri = Glib::filename_to_uri (absolut_name);
		if(Glib::file_test(Glib::filename_from_uri(uri), Glib::FILE_TEST_EXISTS))
		{
		    std::cout << "Trying to open: " << uri << std::endl;
			return uri;
		}
		else
		{
		    std::cout << "No valid uri: " << uri << std::endl;
			return "";
		}
	}
	catch (const Glib::ConvertError &ex)
	{   // argv is a uri
		std::cout << ex.what() << std::endl;
	    std::cout << "No valid uri: " << uri << std::endl;
		return "";
	}
	
}

