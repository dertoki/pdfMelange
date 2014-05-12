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

#include "melangePassword.h"
#include <libintl.h>
#include <glibmm/i18n.h>

/**
 * \brief Constructor
 *
 *  This creates a password dialog that
 *     prints a message,
 *     presents a test entry,
 *     and ok / cancel buttons.
 *
 * \param const Glib::ustring& title
 * \param Gtk::Window& parent
 */
melangePassword::melangePassword(const Glib::ustring& title, Gtk::Window& parent)
{
    if (&parent) set_transient_for(parent);
    set_title(title);

    Gtk::Box* pVBox = get_content_area ();

    pVBox->pack_start(m_Label);
    pVBox->pack_end(m_Entry);

    Glib::ustring infoText;
    infoText = "\n";
    infoText += "<span foreground=\"red\">";
    infoText += _("Trying to open an encrypted PDF file.");
    infoText += "</span>";
    infoText += "\n\n";
    infoText += "<span size=\"large\">";
    infoText += _("Please enter the password ...");
    infoText += "</span>";

    m_Label.set_markup (infoText);
    m_Label.set_justify (Gtk::Justification::JUSTIFY_LEFT);
    m_Label.set_alignment(0.0);

    m_Entry.set_visibility(false);
    m_Entry.set_invisible_char('X');
    m_Entry.set_activates_default(true);

    add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    m_pOkButton = add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

    show_all();
}

/**
 * \brief Destructor
 */
melangePassword::~melangePassword()
{
}

/**
 * \brief Returns the given password.
 *
 * \return Glib::ustring
 */
Glib::ustring melangePassword::get_password()
{
    return m_Entry.get_text();
}

/**
 * \brief Quit the dialog by responding Gtk::RESPONSE_OK .
 *
 * Override of the default handler for the signal signal_key_press_event().
 *
 * \param GdkEventKey* event
 * \return true when return has been entered.
 */
bool melangePassword::on_key_press_event(GdkEventKey* event)
{
    if(event->keyval == GDK_KEY_Return)
    {
        response(Gtk::RESPONSE_OK);
        return true;
    }

    //if the event has not been handled, call the base class
    return Gtk::Window::on_key_press_event(event);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~| Test Scenario |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~ To create the test scenario, do configure pdfmelange with "./configure --enable-testapps" ~*/

#ifdef TEST_PASSWORD

#include <iostream>

#ifdef __WIN32
	#define GETTEXT_PACKAGE "pdfmelange"
	#define PROGRAMNAME_LOCALEDIR "../share/locale"
#else
	#include "config.h"
#endif

/**
 *  \brief Entry point of the test program.
 *
 *  Workflow:
 *  1. bind to locale language.
 *  2. instantiate melangePassword.
 *  3. run the dialog.
 *
 * \param argc: argument count, number of arguments on the command line.
 * \param *argv[]: argument vector, array of null-terminated strings,
 *                 representing the arguments that were entered on the command line
 *                 when the program was started.
 * \return -
 */
int main (int argc, char *argv[])
{
    /** start bind locales */
    bindtextdomain(GETTEXT_PACKAGE, PROGRAMNAME_LOCALEDIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);
    setlocale (LC_ALL, "");
    /** end bind locales */

    Gtk::Main kit(argc, argv);

    Gtk::Window *parent = NULL;
    melangePassword dialog("Password request dialog", *parent);

    switch(dialog.run())
    {
    case(Gtk::RESPONSE_OK):
    {
        std::cout << "OK clicked." << std::endl;
        std::cout << dialog.get_password().c_str() << std::endl;
        break;
    }
    case(Gtk::RESPONSE_CANCEL):
    {
        std::cout << "Cancel clicked." << std::endl;
        break;
    }
    default:
    {
        std::cout << "Unexpected button clicked." << std::endl;
        break;
    }
    }

    return 0;
}

#endif