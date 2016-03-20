/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * melangeTreeView.cc
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

#include "melangeTreeView.h"
#include "melangePassword.h"

#include <libintl.h>
#include <glibmm/i18n.h>
#include <gtkmm.h>


/**
 * \brief Constructor
 */
melangeTreeView::melangeTreeView()
{
    init();
}

/**
 * \brief Constructor for Gtk::Builder
 *
 * 	  m_pTreeView = NULL;
 *    m_refGlade->get_widget_derived("TreeView1", m_pTreeView);
 *
 */
melangeTreeView::melangeTreeView(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder)
    : Gtk::TreeView(cobject)
{
    init();
}

/**
 * \brief Destructor
 */
melangeTreeView::~melangeTreeView()
{
}

/**
 * \brief Initialization function for the constructors.
 *
 * Set initial values, handling of signals and events.
 */
void melangeTreeView::init()
{
    m_refSelection = this->get_selection();
    m_refSelection->set_mode(Gtk::SELECTION_MULTIPLE);
    m_refSelection->signal_changed().connect(sigc::mem_fun(this, &melangeTreeView::on_changed) );
    m_refBuffer = melangeListStore::create();

    // DnD targets.
    std::vector<Gtk::TargetEntry> listTargets;
    listTargets.push_back( Gtk::TargetEntry("GTK_TREE_MODEL_ROW", Gtk::TARGET_SAME_WIDGET, INFO_PDF_LISTSTORE) );
    listTargets.push_back( Gtk::TargetEntry("text/uri-list", Gtk::TARGET_OTHER_APP, INFO_PDF_URI_LIST));
    this->drag_source_set(listTargets);
    this->drag_dest_set(listTargets);

    //Connect activities and methods for DnD.
    this->signal_drag_data_received().connect(sigc::mem_fun(*this, &melangeTreeView::on_drop_drag_data_received) );
    this->signal_drag_data_get().connect(sigc::mem_fun(*this, &melangeTreeView::on_drag_data_get));
    this->signal_drag_motion().connect(sigc::mem_fun(*this, &melangeTreeView::on_drag_motion));

    //Fill popup menu:
    Gtk::MenuItem* item;
    item = Gtk::manage(new Gtk::MenuItem(_("Rotate CW"), true));
    item->signal_activate().connect( sigc::mem_fun(*this, &melangeTreeView::on_rotate_cw_request) );
    m_Menu_Popup.append(*item);

    item = Gtk::manage(new Gtk::MenuItem(_("Rotate CCW"), true));
    item->signal_activate().connect( sigc::mem_fun(*this, &melangeTreeView::on_rotate_ccw_request) );
    m_Menu_Popup.append(*item);

    item = Gtk::manage(new Gtk::MenuItem(_("_Copy"), true));
    item->signal_activate().connect( sigc::mem_fun(*this, &melangeTreeView::copy_selected) );
    m_Menu_Popup.append(*item);

    item = Gtk::manage(new Gtk::MenuItem(_("Cu_t"), true));
    item->signal_activate().connect( sigc::mem_fun(*this, &melangeTreeView::cut_selected) );
    m_Menu_Popup.append(*item);

    item = Gtk::manage(new Gtk::MenuItem(_("_Delete"), true));
    item->signal_activate().connect( sigc::mem_fun(*this, &melangeTreeView::delete_selected) );
    m_Menu_Popup.append(*item);

    item = Gtk::manage(new Gtk::MenuItem(_("_Paste"), true));
    item->signal_activate().connect( sigc::mem_fun(*this, &melangeTreeView::paste) );
    m_Menu_Popup.append(*item);

    m_Menu_Popup.accelerate(*this);
    m_Menu_Popup.show_all(); //Show all menu items when the menu pops up
}

/**
 * \brief Overwrite of the default method.
 *
 * This also stores model as melangeListStore as member variable.
 * 
 * \param const Glib::RefPtr<melangeListStore>& model: the data model
 * \return void
 */
void melangeTreeView::set_model(const Glib::RefPtr<melangeListStore>& model)
{
    m_refModel = model;
    Gtk::TreeView::set_model(model);
    
    //Add the TreeView's view columns:    
    append_column("", m_refModel->m_Columns.thumbnail);
    append_column(_("file"), m_refModel->m_Columns.filename);
    append_column(_("page"), m_refModel->m_Columns.page);
    append_column(_("rotation"), m_refModel->m_Columns.angle);
}

/**
 * \brief Overwrite of the default method.
 *
 * \return model: Glib::RefPtr<const melangeListStore>
 */
Glib::RefPtr<const melangeListStore> melangeTreeView::get_model()
{
    return m_refModel;
}

/**
 * \brief Print the selected rows with g_message.
 *
 * \return void
 */
void melangeTreeView::print_selected()
{
    // get selection
    std::vector<Gtk::TreeModel::Path> pathlist = m_refSelection->get_selected_rows();

    // Print selection with g_message
    for(gint ip = 0; ip < pathlist.size(); ip++)
    {
        Gtk::TreeModel::iterator iter = m_refModel->get_iter(pathlist[ip]);
        Gtk::TreeModel::Row row = *iter;

        Glib::ustring name = row[m_refModel->m_Columns.filename];
        int i = row[m_refModel->m_Columns.page];
        Glib::RefPtr<Gdk::Pixbuf> pixbuf = row[m_refModel->m_Columns.thumbnail];
        Glib::ustring pathName = (pathlist[ip]).to_string();
        g_message("   TreePath %s file <%s> page <%i> pixbuf: width <%i> height <%i>", pathName.c_str(), name.c_str(), i, pixbuf->get_width(), pixbuf->get_height());
    }
}

/**
 * \brief Delete all rows.
 *
 * \return void
 */
void melangeTreeView::clear()
{
    m_refSelection->set_mode(Gtk::SELECTION_NONE);
    m_refModel->clear();
    m_refSelection->set_mode(Gtk::SELECTION_MULTIPLE);
    signal_list_modified.emit();
}

/**
 * \brief Delete the selected rows.
 *
 * \return void
 */
void melangeTreeView::delete_selected()
{
    std::vector<Gtk::TreeModel::Path> pathlist = m_refSelection->get_selected_rows();
    if (pathlist.size()==0) 
        return;

    m_refSelection->set_mode(Gtk::SELECTION_NONE);

    this->unset_cursor();

    for(gint ip = pathlist.size()-1; ip >= 0; ip--)
    {
        Gtk::TreeModel::iterator iter = m_refModel->get_iter(pathlist[ip]);
        Gtk::TreeModel::Row row = *iter;

        Glib::ustring name = row[m_refModel->m_Columns.filename];
        int i = row[m_refModel->m_Columns.page];
        Glib::ustring pathName = (pathlist[ip]).to_string();

        g_message("   delete in path %s page %i file %s",  pathName.c_str(), i, name.c_str());

        m_refModel->erase(iter);
    }
    m_refSelection->set_mode(Gtk::SELECTION_MULTIPLE);
    signal_list_modified.emit();
}

/**
 * \brief Delete the selected rows.
 *
 * \param bool select: unselect it false
 * \return void
 */
void melangeTreeView::select_all(bool select = true )
{
    if (select)
        m_refSelection->select_all();
    else
        m_refSelection->unselect_all();        
}

/**
 * \brief Copy the selected rows.
 *
 * \return void
 */
void melangeTreeView::copy_selected()
{
    g_message("melangeTreeView::copy_selected");

    m_refBuffer->clear();
    std::vector<Gtk::TreeModel::Path> pathlist = m_refSelection->get_selected_rows();

    for(gint ip = 0; ip < pathlist.size(); ip++)
    {
        melangeListStore::Row source = *m_refModel->get_iter(pathlist[ip]);
        melangeListStore::Row dest = *m_refBuffer->append();
        m_refBuffer->set_row(dest, source);
    }
}

/**
 * \brief Cut the selected rows.
 *
 * \return void
 */
void melangeTreeView::cut_selected()
{
    copy_selected ();
    delete_selected ();
}

/**
 * \brief Paste the buffer.
 *
 * \return void
 */
void melangeTreeView::paste()
{
    g_message("melangeTreeView::paste");

    std::vector<Gtk::TreeModel::Path> pathlist = m_refSelection->get_selected_rows();
    m_refSelection->unselect_all();

    if (pathlist.size())  // insert Buffer after selected row in Model
    {
        melangeListStore::iterator iter_dest = m_refModel->get_iter(pathlist[pathlist.size()-1]);

        for (melangeListStore::iterator iter = m_refBuffer->children().begin(); iter != m_refBuffer->children().end(); iter++)
        {
            melangeListStore::Row dest = *m_refModel->insert_after(iter_dest);
            melangeListStore::Row source = *iter;
            m_refModel->set_row(dest, source);
            m_refSelection->select(dest);
            iter_dest++;
        }
    }
    else // append Buffer to the end of Model
    {
        for (melangeListStore::iterator iter = m_refBuffer->children().begin(); iter != m_refBuffer->children().end(); iter++)
        {
            melangeListStore::Row dest = *m_refModel->append();
            melangeListStore::Row source = *iter;
            m_refModel->set_row(dest, source);
            m_refSelection->select(dest);
        }
    }
    signal_list_modified.emit();
}

/**
 * \brief Signal handler for signal_changed().
 *
 *    Check for selection and send signal_selected with the number of selected rows.
 *
 * \return void
 */
void melangeTreeView::on_changed()
{
    g_message("melangeTreeView::on_changed");

    //get the selection
    std::vector<Gtk::TreeModel::Path> pathlist;
    pathlist = m_refSelection->get_selected_rows();

    signal_selected.emit(pathlist.size());
}

/**
 * \brief Check if first row has been selected.
 *
 *    Check for selection of the first row.
 *
 * \return true if selected, else false 
 */
bool melangeTreeView::is_selected_first_row(){
    
    std::vector<Gtk::TreeModel::Path> selected_rows = m_refSelection->get_selected_rows();

    if (m_refModel->get_iter(selected_rows.front()) == m_refModel->children().begin()) {
        return true;
    }
    else {
        return false;
    }
};

/**
 * \brief Check if last row has been selected.
 *
 *    Check for selection of the last row.
 *
 * \return true if selected, else false 
 */
bool melangeTreeView::is_selected_last_row(){

    std::vector<Gtk::TreeModel::Path> selected_rows = m_refSelection->get_selected_rows();

    if (m_refModel->get_iter(selected_rows.back()) == --m_refModel->children().end()) {
        return true;
    }
    else {
        return false;
    }
};


/**
 * \brief Append a pdf to the end of the list.
 *
 *   Ask for a file.
 * 
 * \param Glib::ustring uri: the pdf file to be appended.
 * \return void
 */
void melangeTreeView::append(Glib::ustring uri)
{
    g_message("melangeTreeView::append");

    int old_size = m_refModel->children().size();
    Gtk::TreeModel::iterator iter;
    get_pdf_document(uri, iter);
    if (old_size) signal_list_modified.emit();
}

/**
 * \brief Insert a pdf after the current row.
 *
 *   Ask for a file.
 * 
 * \param Glib::ustring uri: the pdf file to be inserted.
 * \return void
 */
void melangeTreeView::insert(Glib::ustring uri)
{
    g_message("melangeTreeView::insert");

    std::vector<Gtk::TreeModel::Path> path = this->get_selection()->get_selected_rows();

    if (path.size() > 0)
    {
        Gtk::TreeModel::iterator iter = m_refModel->get_iter(path[0]);
        get_pdf_document(uri.c_str(), iter, Gtk::TREE_VIEW_DROP_AFTER);
        signal_list_modified.emit();
    }
    else
        append(uri);
}

/**
 * \brief Generate the thumbnails for a pdf.
 *
 *      To append the file to the end of the model create a non valid iterator as follows:
 *            Gtk::TreeModel::iterator iter;
 *
 * \param Glib::ustring uri: the pdf file to be inserted.
 * \param Gtk::TreeModel::iterator &iter: the position in the current model where the insert is done.
 * \param Gtk::TreeViewDropPosition pos: indicates weather to inert before or after iter.
 * \return void
 */
void melangeTreeView::get_pdf_document(Glib::ustring uri,
                                       Gtk::TreeModel::iterator &iter,
                                       Gtk::TreeViewDropPosition pos
                                       )
{
    g_message("melangeTreeView::get_pdf_document");

    Glib::ustring password;
    GError* error;
    melangeListStore::sPermissions = POPPLER_PERMISSIONS_FULL;
    melangeListStore::sEncrypted = false;

    signal_idle.emit( true );
    do {
        /** read the the file into the model */
        error = m_refModel->read_pdf_document(uri, password, iter, pos);

        if (error && error->code == POPPLER_ERROR_ENCRYPTED) // handle encrypted files
        {
            g_message("   Error Nr.%i, %s\n", error->code, error->message);
            melangePassword dialog("Password request dialog", (Gtk::Window&)(*this->get_toplevel()));
            switch(dialog.run())
            {
                case(Gtk::RESPONSE_OK):
                {
                    password = strdup(dialog.get_password().c_str());
                    break;
                }
                case(Gtk::RESPONSE_CANCEL):
                {
                    error = NULL;
                    break;
                }
                default:
                {
                    std::cout << "Unexpected button clicked." << std::endl;
                    error = NULL;
                    break;
                }
            }
        } 
        else if (error && error->code == MELANGE_ERROR_PERMISSIONS) // handle pdf permissions
        {
            g_message("  Error Nr.%i, %s\n", error->code, error->message);

            Glib::ustring message;
            message =  _("Error when opening a PDF file with restricted permissions.");
            message += "\n";
            message += _("Restricted permissions are not supported yet.");
            Gtk::MessageDialog messagedialog((Gtk::Window&)(*this->get_toplevel()),
                                             message,
                                             false, /* use_markup */
                                             Gtk::MESSAGE_ERROR,
                                             Gtk::BUTTONS_CLOSE);
            messagedialog.run();

            error = NULL;
        }
        else // e.g. file with wrong extension. 
        {
            error = NULL;
        }
    } while (error);
    signal_idle.emit( false );
}

/**
 * \brief Moves pages upward. Decrements the selected indices in the model.    
 */
void melangeTreeView::move_selected_up()
{
    g_message("melangeTreeView::on_action_move_page_up");

    std::vector<Gtk::TreeModel::Path> pathlist = m_refSelection->get_selected_rows();
    
    m_refModel->move_1minus(pathlist);

    pathlist = get_selection()->get_selected_rows();
    Gtk::TreeModel::iterator iter = m_refModel->get_iter(pathlist.front());
    scroll_to_row(m_refModel->get_path(iter));

    signal_list_modified.emit();
}

/**
 * \brief Moves pages downward. Increments the selected indices in the model.
 */
void melangeTreeView::move_selected_down()
{
    g_message("melangeTreeView::on_action_move_page_down");

    std::vector<Gtk::TreeModel::Path> pathlist = m_refSelection->get_selected_rows();
    
    m_refModel->move_1plus(pathlist);

    pathlist = get_selection()->get_selected_rows();
    Gtk::TreeModel::iterator iter = m_refModel->get_iter(pathlist.back());
    scroll_to_row(m_refModel->get_path(iter));

    signal_list_modified.emit();
}

/**
 * \brief Select next page.
 */
void melangeTreeView::select_next()
{
    g_message("melangeTreeView::select_next");

    std::vector<Gtk::TreeModel::Path> pathlist = get_selection()->get_selected_rows();
    Gtk::TreeModel::iterator iter = m_refModel->get_iter(pathlist[0]);

    if (pathlist.size() == 0 || pathlist.size() > 1) return;
    
    if (++iter) {
        m_refSelection->unselect_all();
        m_refSelection->select(iter);
        scroll_to_row(m_refModel->get_path(iter));
    }
}

/**
 * \brief Select previous page.
 */
void melangeTreeView::select_previous()
{
    g_message("melangeTreeView::select_previous");

    std::vector<Gtk::TreeModel::Path> pathlist = get_selection()->get_selected_rows();
    Gtk::TreeModel::iterator iter = m_refModel->get_iter(pathlist[0]);

    if (pathlist.size() == 0 || pathlist.size() > 1) return;
    
    if (--iter) {
        m_refSelection->unselect_all();
        m_refSelection->select(iter);
        scroll_to_row(m_refModel->get_path(iter));
    }
}

/**
 * \brief Rotate the selected elements clockwise.
 */
void melangeTreeView::rotate_selected_cw()
{
    g_message("melangeTreeView::rotate_selected_cw");

    std::vector<Gtk::TreeModel::Path> pathlist;
    pathlist = m_refSelection->get_selected_rows();
    m_refModel->rotate_cw(pathlist);
    signal_list_modified.emit();
}

/**
 * \brief Rotate the selected elements counterclockwise.
 */
void melangeTreeView::rotate_selected_ccw()
{
    g_message("melangeTreeView::rotate_selected_ccw");

    std::vector<Gtk::TreeModel::Path> pathlist;
    pathlist = m_refSelection->get_selected_rows();
    m_refModel->rotate_ccw(pathlist);
    signal_list_modified.emit();
}

/**
 * \brief Rotate the page clockwise.
 */
void melangeTreeView::on_rotate_cw_request()
{
    g_message("melangeTreeView::on_rotate_cw_request");
	signal_rotate_cw_request.emit();
}

/**
 * \brief Rotate the page counter clockwise.
 */
void melangeTreeView::on_rotate_ccw_request()
{
    g_message("melangeTreeView::on_rotate_ccw_request");
	signal_rotate_ccw_request.emit();
}

/**
 * \brief signal handler signal_drag_data_get().
 */
void melangeTreeView::on_drag_data_get(const Glib::RefPtr<Gdk::DragContext>&,
                                     Gtk::SelectionData& selection_data, guint, guint)
{
    g_message("melangeTreeView::on_drag_data_get");
    g_message("   selection_data: %s", selection_data.get_target().c_str());

    melangeListStore* pStore= m_refModel.operator->();
    selection_data.set(selection_data.get_target(), 32,  (const guint8*) &pStore, sizeof(Glib::RefPtr<Gtk::TreeModel>));
}

/**
 * \brief The signal handler for signal_drag_data_received().
 */
void melangeTreeView::on_drop_drag_data_received(
    const Glib::RefPtr<Gdk::DragContext>& context, int x, int y,
    const Gtk::SelectionData& selection_data, guint info, guint time)
{
    g_message("melangeTreeView::on_drop_drag_data_received");
    g_message("   data_type = %s", selection_data.get_data_type().c_str());
    g_message("   data info = %i", info);

    switch (info)
    {
        case  INFO_PDF_LISTSTORE:
        {
            // selectiondata is a pointer to TreeView. Use the pointer to check the source.
            // Have a look at the drag_data_get() signal handlers.
            melangeListStore **pModel = (melangeListStore**) selection_data.get_data();
            melangeListStore *source_model = *pModel;
            melangeListStore *dest_model = m_refModel.operator->();

            /*
             * Change / modify the the destination model.
             * First check if same model.
             */
            if (source_model == dest_model)
            {
                /*
                 * This handles DND that remains within the same model with multiple selection.
                 *	The important part is
                 *					get_dest_row_at_pos(x, y, dest_path, pos)
                 *	that is used to determine the correct drop position.
                 */
                g_message("   Drag and Drop in the same model.");

                // source TreeView: get the source path.
                std::vector<Gtk::TreeModel::Path> pathlist = this->get_selection()->get_selected_rows();

                // some debuging output
                for (std::vector<Gtk::TreeModel::Path>::iterator ip = pathlist.begin(); ip != pathlist.end(); ip++)
                {
                    g_message("   source path = %s", (*ip).to_string().c_str());
                }

                // create the vector of indexes for the destination model.
                std::vector<int> dest_indexes;
                Gtk::TreeModel::Children children = dest_model->children();
                Gtk::TreeModel::Children::iterator  child;
                for(child = children.begin(); child != children.end(); ++child)
                {
                    dest_indexes.push_back(dest_model->get_path(child)[0]);
                }

                // create the vector of indexes for the selection.
                std::vector<int> selection_indexes;
                std::vector<Gtk::TreeModel::Path>::iterator path;
                for (path = pathlist.begin(); path != pathlist.end(); ++path)
                {
                    selection_indexes.push_back((*path)[0]);
                }

                // get the destination index.
                int dest_index;
                Gtk::TreeModel::Path dest_path;
                Gtk::TreeViewDropPosition pos;
                if (this->get_dest_row_at_pos(x, y, dest_path, pos))
                {
                    if (pos == Gtk::TREE_VIEW_DROP_BEFORE)
                    {
                        dest_index = dest_path[0];
                    }
                    else
                    {
                        dest_index = dest_path[0] + 1;
                    }
                }
                else
                {
                    dest_index = dest_model->children().size();
                }

                // Modify the index vector of the destination model.
                std::vector<int>::iterator it;
                for (int i = selection_indexes.size() - 1; i >= 0; i--)
                {   // do [destination] minus [selection]
                    if (selection_indexes[i] < dest_index) dest_index--;
                    it = dest_indexes.begin() + selection_indexes.at(i);
                    dest_indexes.erase(it);
                }

                if (dest_index == dest_indexes.size())
                {   // do append [selection] on [destination]
                    for (int i = 0; i < selection_indexes.size(); i++)
                    {
                        dest_indexes.push_back(selection_indexes[i]);
                    }
                }
                else
                {   // do insert [selection] into [destination]
                    dest_indexes.insert(dest_indexes.begin()+dest_index, selection_indexes.begin(), selection_indexes.end());
                }

                // reorder the destination model with the modified index vector.
                dest_model->reorder(dest_indexes);
            }
            else
            {
                /*	todo:
                 *		this is for drops from other models.
                 *		Use the pointer stored in (whatever **) selection_data.get_data() to decide what to do.
                 */
            }
        }
        break;
        case  INFO_PDF_URI_LIST:
        {
            Gtk::TreeModel::Path dest_path;
            Gtk::TreeViewDropPosition pos;
            Gtk::TreeModel::iterator destIter;

            signal_idle.emit( true );

            bool is_dest_row_selected = this->get_dest_row_at_pos(x, y, dest_path, pos);
            if (is_dest_row_selected)
            {
                destIter = m_refModel->get_iter(dest_path);
            }

            std::vector<Glib::ustring> uri_list;
            uri_list = selection_data.get_uris();

            // use filename of DND to set in melangeWindow if melangeListStore is empty.
            if (m_refModel->children().empty())
            {
                Glib::ustring uri = uri_list[0];
                if ( Glib::str_has_suffix(uri, ".pdf") || Glib::str_has_suffix(uri, ".PDF") )
                    signal_new.emit(Glib::filename_from_uri(uri));
            }

            // fill melangeListStore with received files from DND. 
            Gtk::TreeModel::Row destRow;
            for (int i = 0; i < uri_list.size(); i++)
            {
                Glib::ustring uri = uri_list[i];
                if ( Glib::str_has_suffix(uri, ".pdf") || Glib::str_has_suffix(uri, ".PDF") )
                    get_pdf_document(uri.c_str(), destIter, pos);
                else
                    g_message("   error: \"%s\" : file extension incorrect!", uri_list[i].c_str());
            }
            
            signal_idle.emit( false );
        }
        break;
        default:
            g_message("   >> unexpected data_type = %s", selection_data.get_data_type().c_str());
            break;
    }
    // confirm that drag is finished.
    context->drag_finish(false, false, time);

    signal_list_modified.emit();
}

/**
 * \brief The signal handler for signal_drag_motion.
 */
bool melangeTreeView::on_drag_motion(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, guint time)
{
    Gtk::TreeModel::Path path;
    Gtk::TreeViewDropPosition pos;

    if (this->get_dest_row_at_pos(x, y, path, pos))
    {
        //~ Glib::ustring pathName = path.to_string();
        //~ std::cout << "signal_drag_motion = ";
        //~ printf("X%i Y%i  path = %s  %i\n", x, y, pathName.c_str(), pos);
        this->set_drag_dest_row(path, pos);
        return true;
    }
    return false;
}

bool melangeTreeView::on_button_press_event(GdkEventButton* event)
{
  bool return_value = false;

  //Call base class, to allow normal handling,
  //such as allowing the row to be selected by the right-click:
  return_value = TreeView::on_button_press_event(event);

  //Then do our custom stuff:
  if( (event->type == GDK_BUTTON_PRESS) && (event->button == 3) )
  {
    m_Menu_Popup.popup(event->button, event->time);
  }

  return return_value;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~| Test Scenario |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~ To create the test scenario, do configure pdfmelange with "./configure --enable-testapps" ~*/

#ifdef TEST_TREEVIEW

#include <iostream>


/**
 * \class TestWindow
 *
 * \brief Test scenario for melangeTreeView.
 *
 * Workflow:
 *  1. create a main window,
 *    a. add a vertical box, insert a toolbar and a melangeTreeView,
 *    b. add actions to handle signals and events,
 *    c. binds actions with toolitems.
 *  2. load and view the file gpl.pdf from the current directory.
 *
 */
class TestWindow : public Gtk::Window
{
public:
    //Child widgets:
    Gtk::Box m_Box;
    Gtk::Statusbar m_StatusBar;
    Gtk::ScrolledWindow m_ScrolledWindow;
    melangeTreeView m_TreeView;
    Glib::RefPtr<melangeListStore> m_refTreeModel;

    Glib::RefPtr<Gtk::UIManager> m_refUIManager;
    Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;

	Glib::RefPtr<Gtk::Action> m_refAction_Delete;
	Glib::RefPtr<Gtk::Action> m_refAction_SelectAll;
	Glib::RefPtr<Gtk::Action> m_refAction_Copy;
	Glib::RefPtr<Gtk::Action> m_refAction_Cut;
	Glib::RefPtr<Gtk::Action> m_refAction_Paste;
	Glib::RefPtr<Gtk::Action> m_refAction_Rotate_Cw;
	Glib::RefPtr<Gtk::Action> m_refAction_Rotate_Ccw;
	Glib::RefPtr<Gtk::Action> m_refAction_Page_Up;
	Glib::RefPtr<Gtk::Action> m_refAction_Page_Down;

    TestWindow(): m_Box(Gtk::ORIENTATION_VERTICAL)
    {
        set_title("melangeTreeView Test Scenario");
        set_default_size(475, 594);

        // put a MenuBar at the top of the box and other stuff below it.
        add(m_Box);

        //Create actions for menus and toolbars:
        m_refActionGroup = Gtk::ActionGroup::create();

        m_refActionGroup->add( Gtk::Action::create_with_icon_name("Open", "document-open", "_Open", "Open PDF file"),
                              sigc::mem_fun(*this, &TestWindow::on_open)
                              );

        m_refActionGroup->add( Gtk::Action::create_with_icon_name("Quit", "application-exit", "_Quit", "Exit"),
                              sigc::mem_fun(*this, &TestWindow::on_quit)
                              );

        m_refActionGroup->add( m_refAction_Delete = Gtk::Action::create_with_icon_name("DeleteSelected", "list-remove", "_Delete", "Delete selected"), 
                              sigc::mem_fun(m_TreeView, &melangeTreeView::delete_selected)
                              );

        m_refActionGroup->add( Gtk::Action::create_with_icon_name("InsertFile", "list-add", "_Insert", "Insert a file"), 
                              sigc::mem_fun(*this, &TestWindow::on_insert)
                              );

        m_refActionGroup->add( m_refAction_SelectAll = Gtk::Action::create_with_icon_name("SelectAll", "edit-select-all", "_Select_All", "Select all"), 
                              sigc::bind<bool>( sigc::mem_fun(m_TreeView, &melangeTreeView::select_all), true) 
                              );

        m_refActionGroup->add( m_refAction_Copy = Gtk::Action::create_with_icon_name("Copy", "edit-copy", "_Copy", "Copy selected"),
                              sigc::mem_fun(m_TreeView, &melangeTreeView::copy_selected) 
                              );

        m_refActionGroup->add( m_refAction_Cut = Gtk::Action::create_with_icon_name("Cut", "edit-cut", "_Cut", "Cut selected"),
                              sigc::mem_fun(m_TreeView, &melangeTreeView::cut_selected) 
                              );

        m_refActionGroup->add( m_refAction_Paste = Gtk::Action::create_with_icon_name("Paste", "edit-paste", "_Paste", "Paste"),
                              sigc::mem_fun(m_TreeView, &melangeTreeView::paste) 
                              );

        m_refActionGroup->add( m_refAction_Page_Up = Gtk::Action::create_with_icon_name("Down", "go-down", "_Down", "Page down"),
                              sigc::mem_fun(m_TreeView, &melangeTreeView::move_selected_down) 
                              );

        m_refActionGroup->add( m_refAction_Page_Down = Gtk::Action::create_with_icon_name("Up", "go-up", "_Up", "Page up"),
                              sigc::mem_fun(m_TreeView, &melangeTreeView::move_selected_up) 
                              );

        m_refActionGroup->add( m_refAction_Rotate_Ccw = Gtk::Action::create_with_icon_name("RotCcw", "object-rotate-left", "_RotateCcw", "Rotate counter-clockwise"),
                              sigc::mem_fun(m_TreeView, &melangeTreeView::rotate_selected_ccw) 
                              );
        m_TreeView.signal_rotate_ccw_request.connect( sigc::mem_fun(m_TreeView, &melangeTreeView::rotate_selected_ccw) );

        m_refActionGroup->add( m_refAction_Rotate_Cw = Gtk::Action::create_with_icon_name("RotCw", "object-rotate-right", "_RotateCw", "Rotate clockwise"),
                              sigc::mem_fun(m_TreeView, &melangeTreeView::rotate_selected_cw) 
                              );
        m_TreeView.signal_rotate_cw_request.connect( sigc::mem_fun(m_TreeView, &melangeTreeView::rotate_selected_cw) );


        m_TreeView.signal_selected.connect( sigc::mem_fun(*this, &TestWindow::on_selected) );
        m_TreeView.signal_list_modified.connect( sigc::mem_fun(*this, &TestWindow::on_modified) );
        m_TreeView.signal_new.connect( sigc::mem_fun(*this, &TestWindow::on_new) );
        
        m_refUIManager = Gtk::UIManager::create();
        m_refUIManager->insert_action_group(m_refActionGroup);
        add_accel_group(m_refUIManager->get_accel_group());

        //Layout the actions in the toolbar:
        Glib::ustring ui_info = "<ui>"
                                "  <toolbar  name='ToolBar'>"
                                "    <toolitem action='Quit'/>"
                                "    <toolitem action='Open'/>"
                                "    <toolitem action='InsertFile'/>"
                                "    <toolitem action='DeleteSelected'/>"
                                "    <toolitem action='SelectAll'/>"
                                "    <toolitem action='Copy'/>"
                                "    <toolitem action='Cut'/>"
                                "    <toolitem action='Paste'/>"
                                "    <toolitem action='Up'/>"
                                "    <toolitem action='Down'/>"
                                "    <toolitem action='RotCw'/>"
                                "    <toolitem action='RotCcw'/>"
                                "  </toolbar>"
                                "</ui>";
        try
        {
            m_refUIManager->add_ui_from_string(ui_info);
        }
        catch(const Glib::Error& ex)
        {
            std::cerr << "building menus failed: " <<  ex.what();
        }
        //Get the toolbar widgets, and add it to a container widget:
        Gtk::Widget* pToolbar = m_refUIManager->get_widget("/ToolBar") ;
        if(pToolbar)
            m_Box.pack_start(*pToolbar, Gtk::PACK_SHRINK);

        //Only show the scrollbars when they are necessary:
        m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);        
        m_ScrolledWindow.add(m_TreeView);

        m_Box.pack_start(m_ScrolledWindow);
        m_Box.add(m_StatusBar);

        show_all_children();

        //Create the Tree model:
        m_refTreeModel = melangeListStore::create();
        m_TreeView.set_model(m_refTreeModel);

        // set sensitive for some icons
        on_new("no file");
    };

    virtual ~TestWindow() {};

    //Signal handlers:
    void on_open()
    {
        Glib::ustring uri;
        if (file_open_dialog(uri)) {
            m_TreeView.append(uri);
        }
        m_TreeView.signal_new.emit(uri);
    };
    void on_append()
    {
        Glib::ustring uri;
        if (file_open_dialog(uri)) {
            m_TreeView.append(uri);
        }
    };
    void on_insert()
    {
        Glib::ustring uri;
        if (file_open_dialog(uri)) {
            m_TreeView.insert(uri);
        }
    };
    void on_quit() {
        hide();
    };
    void on_modified() {
        m_StatusBar.push("file was modified");
    };
    void on_item_modified() {
        m_StatusBar.push("item was modified");
    };
    void on_new(Glib::ustring text) {
        m_StatusBar.push(text);
    };
    void on_selected(int number) {
        m_StatusBar.push(Glib::ustring::compose("%1 Pages selected", number));
        if (number == 0){
            m_refAction_Delete->set_sensitive(false);
	        m_refAction_SelectAll->set_sensitive(true);
	        m_refAction_Copy->set_sensitive(false);
	        m_refAction_Cut->set_sensitive(false);
	        m_refAction_Rotate_Cw->set_sensitive(false);
	        m_refAction_Rotate_Ccw->set_sensitive(false);
	        m_refAction_Page_Up->set_sensitive(false);
	        m_refAction_Page_Down->set_sensitive(false);
        } else {
            m_refAction_Delete->set_sensitive(true);
	        m_refAction_SelectAll->set_sensitive(true);
	        m_refAction_Copy->set_sensitive(true);
	        m_refAction_Cut->set_sensitive(true);
	        m_refAction_Rotate_Cw->set_sensitive(true);
	        m_refAction_Rotate_Ccw->set_sensitive(true);
	        m_refAction_Page_Up->set_sensitive(true);
	        m_refAction_Page_Down->set_sensitive(true);
        }
    };
    bool file_open_dialog(Glib::ustring &uri)
    {
        g_message("melangeTreeView::file_open_dialog");

        Gtk::FileChooserDialog dialog((Gtk::Window&)(*this->get_toplevel()), _("Please choose a file"), Gtk::FILE_CHOOSER_ACTION_OPEN);
        dialog.set_transient_for((Gtk::Window&)(*this->get_toplevel()));

        //Add response buttons the the dialog:
        dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
        dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

        //Add filters, so that only certain file types can be selected:

        Glib::RefPtr<Gtk::FileFilter> filter_pdf = Gtk::FileFilter::create();
        filter_pdf->set_name("PDF files");
        filter_pdf->add_mime_type("application/pdf");
        filter_pdf->add_pattern("*.pdf");
        filter_pdf->add_pattern("*.PDF");
        dialog.add_filter(filter_pdf);
        dialog.set_filter(filter_pdf);

        //Show the dialog and wait for a user response:
        int result = dialog.run();

        //Handle the response:
        switch(result)
        {
            case(Gtk::RESPONSE_OK):
            {
                g_message("   File selected: %s", dialog.get_filename().c_str());
                g_message("   URI  selected: %s", dialog.get_uri().c_str());
                uri = dialog.get_uri();
                return true;
                break;
            }
            case(Gtk::RESPONSE_CANCEL):
            {
                return false;
                break;
            }
            default:
            {
                g_message("   Unexpected button clicked.");
                return false;
                break;
            }
        }
    };
};

/**
 *  \brief Entry point of the test program.
 *
 *  Workflow:
 *  1. instantiate a gtk main window.
 *  2. start gtk main loop.
 *  3. clean up.
 *
 * \param argc: argument count, number of arguments on the command line.
 * \param *argv[]: argument vector, array of null-terminated strings,
 *                 representing the arguments that were entered on the command line
 *                 when the program was started.
 * \return -
 */
int main (int argc, char *argv[])
{

    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "melange.test.scenario");
    TestWindow window;

    //Shows the window and returns when it is closed.
    app->run(window);


    return 0;
}

#endif