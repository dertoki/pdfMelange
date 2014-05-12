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

#ifndef _MELANGEPASSWORD_H_
#define _MELANGEPASSWORD_H_

#include <gtkmm.h>

/**
 * \class melangePassword
 *
 * \brief A simple password request dialog.
 *
 * A dialog that is created as follows:
 *
 *    Gtk::Main kit(argc, argv);
 *    Gtk::Window *parent = NULL;
 *    melangePassword dialog("Password request dialog", *parent);
 *    if (dialog.run() == Gtk::RESPONSE_OK) {std::cout << dialog.get_password().c_str() << std::endl;}
 *
 */
class melangePassword: public Gtk::Dialog
{
public:
    melangePassword(const Glib::ustring& title, Gtk::Window& parent);
    ~melangePassword();
    Glib::ustring get_password();

protected:

private:
    Gtk::Label m_Label;
    Gtk::Entry m_Entry;
    Gtk::ButtonBox m_BBox;
    Gtk::Button *m_pOkButton;
    virtual bool on_key_press_event(GdkEventKey* event);
};

#endif // _MELANGEPASSWORD_H_
