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

/*
 * derived from:
 * http://git.gnome.org/browse/gtkmm-documentation/tree/examples/book/treeview/drag_and_drop/treemodel_dnd.h?h=gtkmm-2-24
 */

//#include "melangePyPdfWriter.h"
#include "melangePopplerWriter.h"

#include "melangeListStore.h"
#include "melangeWindow.h"

#include <glib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <iostream>

PopplerPermissions melangeListStore::sPermissions = POPPLER_PERMISSIONS_FULL;
bool melangeListStore::sEncrypted = false;

/**
 * \brief Default constructor
 */
melangeListStore::melangeListStore()
{
    set_column_types(m_Columns);
    m_iconSize = 60;
}

/**
 * \brief Default destructor
 */
melangeListStore::~melangeListStore()
{
}

/**
 * \brief Instantiate a new ListStore.
 */
Glib::RefPtr<melangeListStore> melangeListStore::create()
{
    return Glib::RefPtr<melangeListStore>( new melangeListStore() );
}

/**
 * \brief Set the values of a row.
 *
 * Copy values from source to dest.
 *
 * \param melangeListStore::Row& dest
 * \param melangeListStore::Row& source
 * \return void
 */
void melangeListStore::set_row(melangeListStore::Row &dest, melangeListStore::Row &source)
{
    dest.set_value(m_Columns.page,      static_cast<int>(source[m_Columns.page]));
    dest.set_value(m_Columns.angle,     static_cast<int>(source[m_Columns.angle]));
    dest.set_value(m_Columns.uri,       static_cast<Glib::ustring>(source[m_Columns.uri]));
    dest.set_value(m_Columns.password,  static_cast<Glib::ustring>(source[m_Columns.password]));
    dest.set_value(m_Columns.filename,  static_cast<Glib::ustring>(source[m_Columns.filename]));
    dest.set_value(m_Columns.thumbnail, static_cast<Glib::RefPtr<Gdk::Pixbuf>>(source[m_Columns.thumbnail]));
};


/**
 * \brief Rotate the specified elements clockwise.
 *
 * \param std::vector<Gtk::TreeModel::Path> pathlist
 * \return void
 */
void melangeListStore::rotate_cw( std::vector<Gtk::TreeModel::Path> pathlist )
{
    for(gint ip = 0; ip < pathlist.size(); ip++)
    {
        Gtk::TreeModel::iterator iter = get_iter(pathlist[ip]);
        Gtk::TreeModel::Row row = *iter;

        row[m_Columns.thumbnail] = ((Glib::RefPtr<Gdk::Pixbuf>)row[m_Columns.thumbnail])->rotate_simple (Gdk::PIXBUF_ROTATE_CLOCKWISE);
        row[m_Columns.angle] = (row[m_Columns.angle] + 90) % 360;
    }
}

/**
 * \brief Rotate the specified elements counterclockwise.
 *
 * \param std::vector<Gtk::TreeModel::Path> pathlist
 * \return void
 */
void melangeListStore::rotate_ccw( std::vector<Gtk::TreeModel::Path> pathlist )
{
    for(gint ip = 0; ip < pathlist.size(); ip++)
    {
        Gtk::TreeModel::iterator iter = get_iter(pathlist[ip]);
        Gtk::TreeModel::Row row = *iter;

        row[m_Columns.thumbnail] = ((Glib::RefPtr<Gdk::Pixbuf>)row[m_Columns.thumbnail])->rotate_simple (Gdk::PIXBUF_ROTATE_COUNTERCLOCKWISE);
        row[m_Columns.angle] = (row[m_Columns.angle] + 270) % 360;
    }
}

/**
 * \brief Increment the position of specified elements.
 *
 * \param std::vector<Gtk::TreeModel::Path> pathlist
 * \return void
 */
void melangeListStore::move_1plus( std::vector<Gtk::TreeModel::Path> pathlist )
{
    Gtk::TreeModel::Children list=children();
    int lastIdx = pathlist[pathlist.size()-1][0];
    if (lastIdx < list.size() - 1)
    {
        for (gint i = (pathlist.size() - 1); i >= 0; i--) {
            Gtk::TreeModel::Path iPath1 = pathlist[i];
            Gtk::TreeModel::Path iPath2 = pathlist[i];
            iPath2.next();
            Gtk::TreeModel::iterator iter1 = get_iter(iPath1);
            Gtk::TreeModel::iterator iter2 = get_iter(iPath2);
            iter_swap(iter1, iter2);
        }
    }
}

/**
 * \brief Decrement the position of specified elements.
 *
 * \param std::vector<Gtk::TreeModel::Path> pathlist
 * \return void
 */
void melangeListStore::move_1minus( std::vector<Gtk::TreeModel::Path> pathlist )
{
    // replace each selected element with its preceding element
    for (gint i = 0; i < pathlist.size(); i++) {
        Gtk::TreeModel::Path iPath1 = pathlist[i];
        if (iPath1.prev())
        {
            Gtk::TreeModel::Path iPath2 = pathlist[i];
            Gtk::TreeModel::iterator iter1 = get_iter(iPath1);
            Gtk::TreeModel::iterator iter2 = get_iter(iPath2);
            iter_swap(iter1, iter2);
        }
    }
}

/**
 * \brief List all elements to std::out
 */
void melangeListStore::print()
{
    Gtk::TreeModel::Children list = children();
    Gtk::TreeModel::Children::iterator iter;

    for (iter = list.begin() ; iter != list.end(); iter++)
    {
        print(iter);
    }
}

/**
 * \brief List a single element to std::out
 *
 * \param Gtk::TreeModel::Children::iterator iter
 * \return void
 */
void melangeListStore::print( const Gtk::TreeModel::Children::iterator iter )
{
    Gtk::TreeModel::Row row;
    Glib::RefPtr<Gdk::Pixbuf> pixbuf;

    row = *iter;
    Glib::ustring name = row[m_Columns.filename];
    int i = row[m_Columns.page];
    pixbuf = row[m_Columns.thumbnail];
    printf("> file <%s> page <%i> pixbuf: width <%i> height <%i>\n", name.c_str(), i, pixbuf->get_width(), pixbuf->get_height());
}


/**
 * \brief Open a PDF document and create list elements for each page.
 *
 * \param Glib::ustring& uri of the pdf file.
 * \param Glib::ustring& password of the pdf file.
 * \param Gtk::TreeModel::iterator& iter a new list item.
 * \param Gtk::TreeViewDropPosition pos
 *	      The position of the new can be
 *           * at the end of the list -- when iter isn't valid,
 *           * in front of iter -- when pos == Gtk::TREE_VIEW_DROP_BEFORE
 *           * after iter -- when pos != Gtk::TREE_VIEW_DROP_BEFORE
 * \return GError* NULL if no error, else an error passed by poppler or a new error.
 */
GError* melangeListStore::read_pdf_document(
											Glib::ustring &uri,
											Glib::ustring &password,
											Gtk::TreeModel::iterator &iter,
											Gtk::TreeViewDropPosition pos )
{
    //Open a PDF and create all thumbnails
#define LINEWIDTH 10

    PopplerDocument* pDoc;
    PopplerPage* pPage;
    GError* err = NULL;

    pDoc = poppler_document_new_from_file(uri.c_str(), password.c_str(), &err);
    if (err)
    {
        sEncrypted = true;
        return err;
    }

    PopplerPermissions permissions = poppler_document_get_permissions(pDoc);
    if (!(permissions & POPPLER_PERMISSIONS_OK_TO_MODIFY))
    {
        sPermissions = permissions;
        return g_error_new (
                   g_quark_from_string("melangeError"),
                   MELANGE_ERROR_PERMISSIONS,
                   "melangeError: no permission to modify document \"%s\".",
                   uri.c_str()
               );
    }


    if (pDoc)
    {
        while (Gtk::Main::events_pending ()) Gtk::Main::iteration ();
        int pages;
        double pdfwidth, pdfheight, thumpScale;
        int width, height;

        Gtk::TreeModel::Row row;
        pages = poppler_document_get_n_pages(pDoc);
        for (int i=0; i < pages; i++)
        {
            while (Gtk::Main::events_pending ()) Gtk::Main::iteration ();

            pPage = poppler_document_get_page(pDoc, i);
            poppler_page_get_size(pPage, &pdfwidth, &pdfheight);

            if (pdfheight > pdfwidth)
            {
                height = m_iconSize;
                thumpScale = height/pdfheight;
                width = static_cast<int>(height*pdfwidth/pdfheight);
            }
            else
            {
                width = m_iconSize;
                thumpScale = width/pdfwidth;
                height = static_cast<int>(width*pdfheight/pdfwidth);
            }

            /// Deprecated poppler_page_render_to_pixbuf, use cairo with Gdk::Pixbuf::create_from_data instead.
            /// poppler_page_render_to_pixbuf (pPage, 0, 0, width, height, pdfScale, 0.0, pixbuf->gobj());

            Cairo::RefPtr<Cairo::ImageSurface> surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, width, height);
            Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(surface);

            cr->scale(thumpScale,thumpScale);
            poppler_page_render(pPage, cr->cobj());
            // With gtk 3.10, this creates an error.
            g_object_unref(pPage);

            /// Draw a black border around the thumbnail.
            cr->set_source_rgb (0.0, 0.0, 0.0);
            cr->set_line_width (LINEWIDTH);
            cr->move_to (0, LINEWIDTH*0.5);
            cr->line_to (pdfwidth-LINEWIDTH, LINEWIDTH*0.5);
            cr->line_to (pdfwidth-LINEWIDTH, pdfheight-LINEWIDTH*0.5);
            cr->line_to (LINEWIDTH*0.5, pdfheight-LINEWIDTH*0.5);
            cr->line_to (LINEWIDTH*0.5, 0);
            cr->stroke ();

            /// for compatibility of colors in poppler and cairo swap blue and red in our ImageSurface.
            //    We have 4 Bytes for each pixel, red / green / blue / alpha (http://en.wikipedia.org/wiki/RGBA_color_space)
            const int rgbSize = 4;
            unsigned int datasize = width*height*rgbSize; // 4 Byte = 32 Bit in FORMAT_ARGB32
            for (unsigned char* ucp = surface->get_data(); ucp < (surface->get_data() + datasize); ucp += rgbSize)
            {
                std::swap(*ucp, *(ucp+2));
            }

            /// Create the pixmap from the cairo surface.
            Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create_from_data(surface->get_data(), Gdk::COLORSPACE_RGB, true, 8, width, height, surface->get_stride());

            /// Create a new list item.
            row = *(new_iterator(iter, pos));
            /// Fill the TreeView's model.
            row[m_Columns.filename] =  Glib::filename_display_basename(Glib::filename_from_uri(uri));
            row[m_Columns.uri] = uri;
            row[m_Columns.password] = password;
            row[m_Columns.permissions] = sPermissions;
            row[m_Columns.encrypted] = sEncrypted;
            row[m_Columns.thumbnail] = pixbuf->copy(); // This copy constructor is necessary to avoid freeing by the reference pointer.
            row[m_Columns.page] = 1+i;
            row[m_Columns.angle] = 0;
        }
        g_object_unref(pDoc); ///Close the PDF
    }
    return err;
}

#include <PDFDoc.h>
#include <GlobalParams.h>
/**
 * /brief Merge all pages of the list, write the merge to a PDF document.
 *
 * \param char* output pdf filename.
 * \return void
 */
void melangeListStore::write_pdf_document( const char *outfilename, melangeKeyFile settings )
{
    g_message("melangeListStore::write_pdf_document");

    Gtk::TreeModel::Children::iterator iter;
    Gtk::TreeModel::Row row;
    melangePopplerWriter pdfWriter;
    //melangePyPdfWriter pdfWriter;

    pdfWriter.setPageModeAndLayout(settings.getPageMode(), settings.getPageLayout());

    for (iter = children().begin() ; iter != children().end(); iter++)
    {
        row = *iter;

        Glib::ustring fileName = Glib::filename_from_uri(row[m_Columns.uri]);
        Glib::ustring passWord = row[m_Columns.password];
        int pageNumber = row[m_Columns.page];
        int pageRotate = row[m_Columns.angle];
        pageItem page = {fileName, passWord, pageNumber, pageRotate};

        pdfWriter.push_back(page);
    }

	pdfWriter.writePdf(outfilename);

	int i = 0;
    for (iter = children().begin() ; iter != children().end(); iter++, i++)
    {
        row = *iter;

		row[m_Columns.filename] =  Glib::filename_display_basename(outfilename);
        row[m_Columns.uri] = Glib::filename_to_uri (outfilename);
        //row[m_Columns.password] = "";
        //row[m_Columns.permissions] = sPermissions;
        //row[m_Columns.encrypted] = sEncrypted;
        row[m_Columns.page] = 1+i;
        row[m_Columns.angle] = 0;
    }
}

/**
 * \brief Create a new iterator.
 *
 * \param Gtk::TreeModel::iterator& iter a new list item.
 * \param Gtk::TreeViewDropPosition pos
 *	      The position of the new can be
 *	         * at the end of the list -- when iter isn't valid,
 * 	         * in front of iter -- when pos == Gtk::TREE_VIEW_DROP_BEFORE
 * 	         * after iter -- when pos != Gtk::TREE_VIEW_DROP_BEFORE
 * \return Gtk::TreeModel::iterator a new iterator
 */
Gtk::TreeModel::iterator melangeListStore::new_iterator(
    Gtk::TreeModel::iterator &iter,
    Gtk::TreeViewDropPosition pos )
{
    Gtk::TreeModel::iterator newiter;
    if (iter)
    {
        if (pos == Gtk::TREE_VIEW_DROP_BEFORE)
            newiter = insert(iter);
        else
        {
            newiter = insert_after(iter);
            iter++;
        }
    }
    else
    {
        newiter = append();
    }
    return newiter;
}

/**
 * \brief Set the size of thumbnails.
 *
 *  This is used in melangeListStore::read_pdf_document, where the thumbnails are created.
 *
 * \param size in pixel.
 * \return void
 */
void melangeListStore::setIconSize(int size)
{
    m_iconSize = size;
}
