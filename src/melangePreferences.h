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

#ifndef _MELANGEPREFERENCES_H_
#define _MELANGEPREFERENCES_H_

#include "melangeKeyFile.h"
#include <gtkmm.h>

/**
 * \class melangePreferences
 *
 * \brief A dialog to edit the melangeKeyFile.
 *
 * A dialog that is created with Gtk::Builder, as follows:
 *
 *    Glib::RefPtr<Gtk::Builder> builder;
 *    melangeKeyFile settings;
 *
 *    #include "melangePreferences.ui-string.h"  // defines: static Glib::ustring ui_builder
 *    builder = Gtk::Builder::create_from_string(ui_builder);
 *    melangePreferences* pDialog = NULL;
 *    builder->get_widget_derived("preferences", pDialog);
 *    settings.loadFromFile();
 *    pDialog->setConfig(settings);
 *    if (pDialog->run() == Gtk::RESPONSE_OK) {settings = pDialog->getConfig(); settings.write();};
 *    delete pDialog;
 *
 */
class melangePreferences: public Gtk::Dialog
{
public:
    melangePreferences(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
    virtual ~melangePreferences();
    void setConfig(melangeKeyFile settings);
    const melangeKeyFile getConfig();

protected:

private:
    //melangeKeyFile m_settings;
    Gtk::Button* m_pButtonSave;
    Gtk::Button* m_pButtonCancel;
    Gtk::CheckButton* m_pCheckButtonLogging;
    Gtk::SpinButton* m_pSpinButtonIconSize;
    Gtk::ComboBoxText* m_pComboPageMode;
    Gtk::ComboBoxText* m_pComboPageLayout;

    void onSave();
    void onCancel();

};

#endif // _MELANGEPREFERENCES_H_
