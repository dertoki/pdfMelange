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

#include "melangeSaveAsDialog.h"
#include <iostream>
#include <libintl.h>

#include <glibmm/i18n.h>

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
melangeSaveAsDialog::melangeSaveAsDialog(const Glib::ustring& title, Gtk::Window* parent)
{
    set_transient_for(*parent);
    m_fileChooser.set_action(Gtk::FILE_CHOOSER_ACTION_SAVE);

    //Add response buttons the the filedialog:
    m_optFrame.set_label("Save with options");
    m_optionsButton.set_label("Extra options");
    get_action_area()->pack_start(m_optionsButton);
    add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    add_button(Gtk::Stock::SAVE_AS, Gtk::RESPONSE_OK);
    set_default_response(Gtk::RESPONSE_OK);

    //Add filters, so that only certain file types can be selected:

    Glib::RefPtr<Gtk::FileFilter> filter_pdf = Gtk::FileFilter::create();
    filter_pdf->set_name("PDF files");
    filter_pdf->add_mime_type("application/pdf");
    filter_pdf->add_pattern("*.pdf");
    filter_pdf->add_pattern("*.PDF");
    m_fileChooser.add_filter(filter_pdf);
    m_fileChooser.set_filter(filter_pdf);

    Gtk::Box* pVBox = get_content_area ();
    pVBox->pack_start(m_fileChooser);

    m_lablePageMode.set_label("Page Mode");
    m_grid.attach(m_lablePageMode, 0, 0, 1, 1);
    m_grid.attach(m_comboPageMode, 1, 0, 1, 1);
    int size = melangeKeyFile::sizeofModes();
    for (int i = 0; i < size; i++) m_comboPageMode.append(melangeKeyFile::m_pageModes[i]);

    m_lablePageLayout.set_label("Page Layout");
    m_grid.attach(m_lablePageLayout, 0, 1, 1, 1);
    m_grid.attach(m_comboPageLayout, 1, 1, 1, 1);
    size = melangeKeyFile::sizeofLayouts();
    for (int i = 0; i < size; i++) m_comboPageLayout.append(melangeKeyFile::m_pageLayouts[i]);

    pVBox->pack_end(m_optFrame, Gtk::PACK_SHRINK );
    m_optFrame.add(m_grid);
    m_grid.set_halign(Gtk::ALIGN_END);
    m_optFrame.set_halign(Gtk::ALIGN_END);

    m_optionsButton.signal_clicked().connect(sigc::mem_fun(*this, &melangeSaveAsDialog::on_button_clicked) );

    show_all();
    m_optionsButton.set_active(false);
    m_optFrame.hide();
}

/**
 * \brief Destructor
 */
melangeSaveAsDialog::~melangeSaveAsDialog()
{
    //dtor
}

/**
 * \brief Signal handler for optionsButton.
 */
void melangeSaveAsDialog::on_button_clicked()
{
    if (m_optionsButton.get_active())
        m_optFrame.show();
    else
        m_optFrame.hide();
}

/**
 * \brief Setter method
 *
 * \param melangeKeyFile settings
 * \return void
 */
void melangeSaveAsDialog::setConfig(melangeKeyFile settings)
{
    m_comboPageLayout.set_active_text(settings.getPageLayout());
    m_comboPageMode.set_active_text(settings.getPageMode());
}

/**
 * \brief Getter method
 *
 * \return melangeKeyFile settings
 */
const melangeKeyFile melangeSaveAsDialog::getConfig()
{
    melangeKeyFile settings;

    settings.setPageLayout(m_comboPageLayout.get_active_text().c_str());
    settings.setPageMode(m_comboPageMode.get_active_text().c_str());

    return settings;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~| Test Scenario |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~ To create the test scenario, do configure pdfmelange with "./configure --enable-testapps" ~*/

#ifdef TEST_SAVEAS

/**
 *  \brief Entry point of the test program.
 *
 *  Workflow:
 *  1. instantiate melangeSaveAsDialog.
 *  2. run the dialog.
 *
 * \param argc: argument count, number of arguments on the command line.
 * \param *argv[]: argument vector, array of null-terminated strings,
 *                 representing the arguments that were entered on the command line
 *                 when the program was started.
 * \return -
 */
int main (int argc, char *argv[])
{
    Gtk::Main kit(argc, argv);

    Gtk::Window window;
    melangeSaveAsDialog dialog(_("Save file"), &window);
    melangeKeyFile settings;
    dialog.setConfig(settings);

    bool doLoop = true;
    while (doLoop) {
        doLoop = false;
        //Show the filedialog and wait for a user response:
        int result = dialog.run();

    }
    return 0;
}
#endif
