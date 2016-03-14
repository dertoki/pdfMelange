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


#ifndef MELANGEWINDOW_H
#define MELANGEWINDOW_H

#include "melangeDrawingArea.h"
#include "melangeTreeView.h"
#include "melangeListStore.h"
#include "melangeKeyFile.h"

#include <gtkmm.h>
#include <sigc++/sigc++.h>
#include <vector>

/**
 * Notation:
 *
 *    m_...    :Member data
 *    m_p...   :Member pointer
 *    m_ref... :Member reference-counting shared smartpointer
 */

/**
 * \class melangeWindow
 *
 * \brief The main application window of pdfMelange.
 *
 * A Gtk::Window widget that is created with Gtk::Builder, as follows:
 *
 *    #include "melange.ui-string.h"  // defines: static Glib::ustring ui_builder
 *    Glib::RefPtr<Gtk::Builder> builder;
 *    builder = Gtk::Builder::create_from_string(ui_builder);
 *    melangeWindow* pMelangeWindow = NULL;
 *    builder->get_widget_derived("main_window", pMelangeWindow);
 *    if (pMelangeWindow) { kit.run(*pMelangeWindow);}
 *    delete pMelangeWindow;
 *
 */
class melangeWindow: public Gtk::Window
{
public:
    melangeWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
    virtual ~melangeWindow();
    void set_pdf_preview();
    void clear_pdf_preview();
	void append_document(Glib::ustring uri);
	void set_mainFileName(std::string);
    void set_programFileName(const char* name);
    void list_the_tree();
    /** Set toolbarbuttons aktiv of inaktiv (set_sensitive) */
    void set_sensitive_on_selection(int number);
    void set_mainFile_is_modified(bool);

protected:
    Glib::RefPtr<melangeListStore> m_refTreeModel;
    Glib::RefPtr<melangeListStore> m_refCopy_Buffer;

private:
    bool m_is_MainFile_modified;
    Glib::ustring m_MainFileName;
    Glib::ustring m_LogFileName;
    Glib::ustring m_ProgramFileName;
    melangeKeyFile m_settings;

    Glib::RefPtr<Gdk::Window> m_refWindow;
    Glib::RefPtr<Gdk::Cursor> m_refCursor;
    Glib::RefPtr<Gtk::Builder> m_refGlade;

    /** Derived widgets */
    melangeDrawingArea* m_pPdfarea;
    melangeTreeView* m_pTreeView;

    /** Glade elements */
    Gtk::Statusbar* m_pStatusbar;
    Gtk::Label* m_pLabelFileInfo;

    Glib::RefPtr<Gtk::Action> m_refPreferences;
    Glib::RefPtr<Gtk::Action> m_refAbout;
    Glib::RefPtr<Gtk::Action> m_refSystemInfo;
    Glib::RefPtr<Gtk::Action> m_refOpen;
    Glib::RefPtr<Gtk::Action> m_refQuit;
    Glib::RefPtr<Gtk::Action> m_refSave;
    Glib::RefPtr<Gtk::Action> m_refSave_as;
    Glib::RefPtr<Gtk::Action> m_refRotate_cw;
    Glib::RefPtr<Gtk::Action> m_refRotate_ccw;
    Glib::RefPtr<Gtk::Action> m_refMove_page_up;
    Glib::RefPtr<Gtk::Action> m_refMove_page_down;
    Glib::RefPtr<Gtk::Action> m_refAppend;
    Glib::RefPtr<Gtk::Action> m_refDelete;
    Glib::RefPtr<Gtk::Action> m_refInsert;
    Glib::RefPtr<Gtk::Action> m_refCopy;
    Glib::RefPtr<Gtk::Action> m_refCut;
    Glib::RefPtr<Gtk::Action> m_refPaste;
    Glib::RefPtr<Gtk::Action> m_refSelect_all;
    Glib::RefPtr<Gtk::Action> m_refUnselect_all;
    Glib::RefPtr<Gtk::Action> m_refZoom_fit;
    Glib::RefPtr<Gtk::Action> m_refZoom_in;
    Glib::RefPtr<Gtk::Action> m_refZoom_out;

    bool file_open_dialog(std::string &filename, Glib::ustring &uri);
	
    /** Signal handlers: */
    void on_action_about();
    void on_action_system_info();
    void on_action_preferences();
    void on_action_insert();
    void on_action_append();
    void on_action_quit();
    void on_action_save();
    void on_action_save_as();
    void write_document();
    void on_action_open();
    void show_idle(bool isIdle = true);
    void on_rotate_cw ();
    void on_rotate_ccw ();
};

#endif // MELANGEWINDOW_H
