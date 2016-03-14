/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * melangeTreeView.h
 * Copyright (C) 2014 Tobias Tandetzki <tobzki@googlemail.com>
 *
 * pdfmelange is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * pdfmelange is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MELANGETREEVIEW_H_
#define _MELANGETREEVIEW_H_

#include "melangeListStore.h"

#include <gtkmm.h>
#include <glib/poppler.h>

/** Notation:
  *
  * m_...    :Member data
  * m_p...   :Member pointer
  * m_ref... :Member reference-counting shared smartpointer
  */

enum info_PDF_Target
{
    INFO_PDF_LISTSTORE 	= 0,		/**< target object of type melangeListStore */
    INFO_PDF_TREESTORE 	= 1,		/**< target object of type PdfTreeStore */
    INFO_PDF_URI_LIST 	= 2			/**< target object of type text/uri-list */
};

/**
 * \class melangeTreeView
 *
 * \brief A Gtk widget to show icons of a pdf files.
 *
 * For an example, see <melangeTreeView.cc> "Test Scenario"
 *
 */

class melangeTreeView: public Gtk::TreeView 
{
public:
    melangeTreeView();
    melangeTreeView(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);
    virtual ~melangeTreeView();
    void set_model( const Glib::RefPtr<melangeListStore>& model);
    Glib::RefPtr<const melangeListStore> get_model();
    void print_selected();
    void clear();
    void delete_selected();
    void select_all(bool select);
    void copy_selected();
    void cut_selected();
    void paste();
    void get_pdf_document( Glib::ustring uri,
                          Gtk::TreeModel::iterator &iter,
                          Gtk::TreeViewDropPosition pos = Gtk::TREE_VIEW_DROP_BEFORE
                          );
    void append(Glib::ustring uri);
    void insert(Glib::ustring uri);
    void move_selected_up();
    void move_selected_down();
    void rotate_selected_cw ();
    void rotate_selected_ccw ();

    sigc::signal<void, int> signal_selected;        // send when selection changed and a number of items are selected.
    sigc::signal<void> signal_list_modified;        // send when the current file was modified
    sigc::signal<void, Glib::ustring> signal_new;   // send when a new file was loaded
    sigc::signal<void, bool> signal_idle;           // send when activity starts (true) / ends (false).
    void on_rotate_cw_request();
    void on_rotate_ccw_request();
    sigc::signal<void> signal_rotate_cw_request;         // send when context popup menu item rotate cw was activated.
    sigc::signal<void> signal_rotate_ccw_request;        // send when context popup menu item rotate ccw was activated.

protected:
    void init();
    virtual bool on_button_press_event(GdkEventButton *ev);
    Gtk::Menu m_Menu_Popup;
    
private:
    Glib::RefPtr<melangeListStore> m_refModel;
    Glib::RefPtr<melangeListStore> m_refBuffer;
    Glib::RefPtr<Gtk::TreeSelection> m_refSelection;    
    void on_selection_changed();
    void on_changed();
	void on_drop_drag_data_received( const Glib::RefPtr<Gdk::DragContext>& context, 
	                                int x, 
	                                int y,
	                                const Gtk::SelectionData& selection_data, 
	                                guint info, 
	                                guint time 
	                                );
	void on_drag_data_get( const Glib::RefPtr<Gdk::DragContext>&,
	                      Gtk::SelectionData& selection_data, 
	                      guint, 
	                      guint 
	                      );
	bool on_drag_motion(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, guint time);
};

#endif // _MELANGETREEVIEW_H_

