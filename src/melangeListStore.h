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

#ifndef MELANGELISTSTORE_H
#define MELANGELISTSTORE_H

#include <gtkmm.h>
#include <fstream>
#include <iostream>
#include <glib/poppler.h>

#define MELANGE_ERROR_PERMISSIONS 42

/**
 * \class melangeListStore
 *
 * \brief A list model for use with a Gtk::TreeView widget.
 *
 * derived from:
 *    http://git.gnome.org/browse/gtkmm-documentation/tree/examples/book/treeview/drag_and_drop/treemodel_dnd.h?h=gtkmm-2-24
 *
 */
class melangeListStore: public Gtk::ListStore
{
public:
    /** Instantiate a new ListStore.  */
    static Glib::RefPtr<melangeListStore> create();

    /**	Create a new iterator.
     *		The position of the new can be
     *			at the end of the list -- when iter isn't valid,
     * 		in front of iter -- when pos == Gtk::TREE_VIEW_DROP_BEFORE
     * 		after iter -- when pos != Gtk::TREE_VIEW_DROP_BEFORE
     */
    Gtk::TreeModel::iterator new_iterator(
        Gtk::TreeModel::iterator &iter,
        Gtk::TreeViewDropPosition pos = Gtk::TREE_VIEW_DROP_BEFORE
    );

    //Tree model columns:
    class ModelColumns : public Gtk::TreeModel::ColumnRecord
    {
    public:
        ModelColumns() {
            add(filename);
            add(thumbnail);
            add(page);
            add(angle);
            add(uri);
            add(password);
            add(permissions);
            add(encrypted);
        }

        Gtk::TreeModelColumn<int> page;
        Gtk::TreeModelColumn<int> angle;
        Gtk::TreeModelColumn<Glib::ustring> uri;
        Gtk::TreeModelColumn<Glib::ustring> filename;
        Gtk::TreeModelColumn<Glib::ustring> password;
        Gtk::TreeModelColumn<PopplerPermissions> permissions;
        Gtk::TreeModelColumn<bool> encrypted;
        Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > thumbnail;
    };
    ModelColumns m_Columns;

    // public methods

    void set_row(melangeListStore::Row &dest, melangeListStore::Row &source);

    /** Rotate the specified elements clockwise. */
    void rotate_cw( std::vector<Gtk::TreeModel::Path> pathlist );

    /** Rotate the specified elements counterclockwise. */
    void rotate_ccw( std::vector<Gtk::TreeModel::Path> pathlist );

    /** Increment the position of specified elements. */
    void move_1plus( std::vector<Gtk::TreeModel::Path> pathlist );

    /** Decrement the position of specified elements. */
    void move_1minus( std::vector<Gtk::TreeModel::Path> pathlist );

    /** List a single element to std::out */
    void print( const Gtk::TreeModel::Children::iterator iter );

    /** List all elements to std::out. */
    void print();

    /** Open a PDF document and create list elements for each page.
     *		The position of the new can be
     *			at the end of the list -- when iter isn't valid,
     * 		in front of iter -- when pos == Gtk::TREE_VIEW_DROP_BEFORE
     * 		after iter -- when pos != Gtk::TREE_VIEW_DROP_BEFORE
     */
    GError* read_pdf_document(
        Glib::ustring &uri,
        Glib::ustring &password,
        Gtk::TreeModel::iterator &iter,
        Gtk::TreeViewDropPosition pos = Gtk::TREE_VIEW_DROP_BEFORE
    );

    /**  Merge all pages of the list, write the merge to a PDF document. */
    void write_pdf_document(const char *outfilename);

    /** Set size of thumbnails. */
    void setIconSize(int size);
    /** Set viewer preferences */
    void setViewerPreferences(const char* PageMode, const char* PageLayout);

    static PopplerPermissions sPermissions;
    static bool sEncrypted;

    /** Default constructor */
    melangeListStore();
    /** Default destructor */
    ~melangeListStore();

private:
    int m_iconSize;
    char* m_strPageMode;
    char* m_strPageLayout;

};

#endif // MELANGELISTSTORE_H
