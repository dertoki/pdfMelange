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

#include "melangePreferences.h"
#include "melangeKeyFile.h"

#include <glib/gstdio.h>
#include <libintl.h>
#include <iostream>

/**
 * \brief Constructor for Gtk::Builder
 *
 *    melangePreferences* pDialog = NULL;
 *    builder->get_widget_derived("preferences", pDialog);
 *
 */
melangePreferences::melangePreferences(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade)
    : Gtk::Dialog (cobject)
{
    // Get the Buttons.

    m_pButtonSave = NULL;
    refGlade->get_widget("save", m_pButtonSave);
    m_pButtonSave->signal_clicked().connect(sigc::mem_fun(this, &melangePreferences::onSave));

    m_pButtonCancel = NULL;
    refGlade->get_widget("cancel", m_pButtonCancel);
    m_pButtonCancel->signal_clicked().connect(sigc::mem_fun(this, &melangePreferences::onCancel));

    // Get gui elements.

    m_pCheckButtonLogging = NULL;
    refGlade->get_widget("checkLogging", m_pCheckButtonLogging);

    m_pSpinButtonIconSize = NULL;
    refGlade->get_widget("spinButtonIconSize", m_pSpinButtonIconSize);

    int size;

    m_pComboPageMode = NULL;
    refGlade->get_widget("comboBoxTextPageMode", m_pComboPageMode);
    size = melangeKeyFile::sizeofModes();
    for (int i = 0; i < size; i++) m_pComboPageMode->append(melangeKeyFile::m_pageModes[i]);

    m_pComboPageLayout = NULL;
    refGlade->get_widget("comboBoxTextPageLayout", m_pComboPageLayout);
    size = melangeKeyFile::sizeofLayouts();
    for (int i = 0; i < size; i++) m_pComboPageLayout->append(melangeKeyFile::m_pageLayouts[i]);

}

/**
 * \brief Destructor
 */
melangePreferences::~melangePreferences()
{
}

/**
 * \brief Signal handler for ButtonSave clicked.
 */
void melangePreferences::onSave()
{
    this->hide();
    this->response(Gtk::RESPONSE_OK);
}

/**
 * \brief Signal handler for ButtonCancel clicked.
 */
void melangePreferences::onCancel()
{
    this->hide();
    this->response(Gtk::RESPONSE_CANCEL);
}

/**
 * \brief Setter method
 *
 * \param melangeKeyFile settings
 * \return void
 */
void melangePreferences::setConfig(melangeKeyFile settings)
{
    m_pCheckButtonLogging->set_active(settings.getLogging());
    m_pSpinButtonIconSize->set_value(settings.getIconSize());
    m_pComboPageLayout->set_active_text(settings.getPageLayout());
    m_pComboPageMode->set_active_text(settings.getPageMode());
}

/**
 * \brief Getter method
 *
 * \return melangeKeyFile settings
 */
const melangeKeyFile melangePreferences::getConfig()
{
    melangeKeyFile settings;

    settings.setLogging(m_pCheckButtonLogging->get_active());
    settings.setIconSize(m_pSpinButtonIconSize->get_value_as_int());
    settings.setPageLayout(m_pComboPageLayout->get_active_text().c_str());
    settings.setPageMode(m_pComboPageMode->get_active_text().c_str());

    return settings;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~| Test Scenario |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~ To create the test scenario, do configure pdfmelange with "./configure --enable-testapps" ~*/

#ifdef TEST_PREF

#include <libintl.h>

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
 *  2. instantiate melangePreferences as derived widget with Gtk::Builder.
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
    /** bind locales */
    bindtextdomain(GETTEXT_PACKAGE, PROGRAMNAME_LOCALEDIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);
    setlocale (LC_ALL, "");

    Gtk::Main kit(argc, argv);

    //Load the Glade file and initiate its widgets:
    Glib::RefPtr<Gtk::Builder> builder;
    try
    {
        /** to create the ui-string header call "gtkbuilder2ustring ../src/melangePreferences.ui ../src/melangePreferences.ui-string.h" */
		#include "melangePreferences.ui-string.h"
        builder = Gtk::Builder::create_from_string(ui_builder);
    }
    catch (const Glib::FileError & ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    melangeKeyFile settings;
    melangePreferences* pDialog = NULL;
    builder->get_widget_derived("preferences", pDialog);

    settings.loadFromFile();
    pDialog->setConfig(settings);

    switch(pDialog->run())
    {
    case(Gtk::RESPONSE_OK):
    {
        settings = pDialog->getConfig();
        printf("logging was set to: %s\n", settings.getLogging() ? "true" : "false");
        printf("Iconsize          : %i\n", settings.getIconSize());
        printf("Page mode         : %s\n", settings.getPageMode());
        printf("Page layout       : %s\n", settings.getPageLayout());
        settings.write();
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

    delete pDialog;

    return 0;
}

#endif