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
#include "config.h"
#include "melangeAboutDialog.h"

#ifdef __WIN32
    #include <windows.h>
    #include <shellapi.h>
#endif

/**
 * \brief Constructor
 *
 * Feed all the setter methods.
 *
 * \param title of the window.
 * \param parent container of widget
 * \param modal If TRUE, other windows are not usable while this one is up.
 * \return void
 */
melangeAboutDialog::melangeAboutDialog(const Glib::ustring& title, Gtk::Window& parent, bool modal)
{
    Glib::ustring progname = PACKAGE_NAME;
    Glib::ustring version  = VERSION;
    Glib::ustring comments = "A program to modify PDF documents.\n"
                             "Merge, erase, reorder and rotate pages.\n"
                             "PdfMelage supprots drag an drop \n"
                             "from your file manager.";
    Glib::ustring copyright = "(C) 2014 by Tobias Tandetzki <tobzki@googlemail.com>";
    Glib::ustring website = "http://dertoki.github.io/pdfMelange";
    Glib::ustring license = "This program is free software; you can redistribute it and/or modify \n"
                            "it under the terms of the GNU General Public License as published by \n"
                            "the Free Software Foundation; either version 3 of the License, or \n"
                            "(at your option) any later version. \n"
                            "\n"
                            "This program is distributed in the hope that it will be useful, \n"
                            "but WITHOUT ANY WARRANTY; without even the implied warranty of \n"
                            "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the \n"
                            "GNU General Public License for more details. \n"
                            "\n"
                            "You should have received a copy of the GNU General Public License \n"
                            "along with this program; if not, write to the \n"
                            "Free Software Foundation, Inc., \n"
                            "59 Temple Place - Suite 330, Boston, MA  02111-1307, USA." ;

    //set_transient_for(*this);
    set_program_name(progname);
	set_logo_default();
    set_version(version);
    set_copyright(copyright);
    set_comments(comments);
    set_license(license);
    set_transient_for(parent);
    set_website(website);
}

/**
 * \brief overwrite the default handler for the signal signal_activate_link().
 *
 * \param uri: start the standard browser to show the uri.
 * \return true
 */
bool melangeAboutDialog::on_activate_link(const std::string& uri)
{
#ifdef __WIN32
    ShellExecute(NULL, "open", uri.c_str(), "", NULL, SW_SHOWNORMAL);
#else
    Gtk::AboutDialog::on_activate_link(uri);
#endif
    return true;
}

/**
 * \brief Destructor
 */
melangeAboutDialog::~melangeAboutDialog()
{
    //dtor
}
