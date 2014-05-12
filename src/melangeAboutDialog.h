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

#ifndef MELANGEABOUTDIALOG_H
#define MELANGEABOUTDIALOG_H

#include <gtkmm.h>


/**
 * \class melangeAboutDialog
 *
 * \brief standard Gtk::AboutDialog
 *
 * Derived from Gtk::AboutDialog, to overwrite the virtual method on_activate_link in case of compiling on Windows OSes.
 *
 */
class melangeAboutDialog : public Gtk::AboutDialog
{
public:
    melangeAboutDialog(const Glib::ustring& title, Gtk::Window& parent, bool modal=false);
    virtual ~melangeAboutDialog();
    bool on_activate_link(const std::string& uri);
protected:
private:
};

#endif // MELANGEABOUTDIALOG_H
