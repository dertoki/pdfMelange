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

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include "melangeKeyFile.h"
#include <gtkmm.h>

class melangeSaveAsDialog : public Gtk::Dialog
{
public:
    melangeSaveAsDialog(const Glib::ustring& title, Gtk::Window* parent);
    virtual ~melangeSaveAsDialog();
    void setConfig(melangeKeyFile settings);
    const melangeKeyFile getConfig();
    const std::string get_filename() {return m_fileChooser.get_filename();};
    const Glib::ustring get_uri() {return m_fileChooser.get_uri();};
    bool set_filename (const std::string& filename) {return m_fileChooser.set_filename(filename);};

protected:
    Gtk::FileChooserWidget m_fileChooser;
    Gtk::Frame m_optFrame;
    Gtk::Grid m_grid;
    Gtk::CheckButton m_optionsButton;
    Gtk::ComboBoxText m_comboPageMode;
    Gtk::ComboBoxText m_comboPageLayout;
    Gtk::Label m_lablePageMode;
    Gtk::Label m_lablePageLayout;

private:
    void on_button_clicked();
};

#endif // MAIN_H_INCLUDED
