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


//#include "Python.h"

#include "melangeWindow.h"
#include "melangeAboutDialog.h"
#include "melangePreferences.h"
#include "melangeLogging.h"
#include "melangePassword.h"
#include "melangeSaveAsDialog.h"

#include <iostream>
#include <gtkmm.h>
#include <glibmm/i18n.h>
#include <gdkmm/cursor.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <libintl.h>
#include <cairo.h>
#include <cairomm/cairomm.h>
#include <poppler/glib/poppler.h>

#ifdef G_OS_UNIX
	#include <sys/utsname.h> // reach the unix function uname(&u_name)
#endif

/**
 * \brief Constructor for Gtk::Builder
 *
 * 	  melangeWindow* pMelangeWindow = NULL;
 *    builder->get_widget_derived("main_window", pMelangeWindow);
 *
 */
melangeWindow::melangeWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade)
    : Gtk::Window (cobject),
      m_refGlade(refGlade),
      m_is_MainFile_modified(true)
{
    //apply configuration settings:
    m_settings.loadFromFile();

    //Set logging.
    melangeLogging::setLogging(m_settings.getLogging());
    m_LogFileName = melangeLogging::getLogFileName();

    //This is where we draw on the window.
    m_refWindow = get_window();

    //Get a drawingareaPDF.
    m_pPdfarea = NULL;
    m_refGlade->get_widget_derived("drawingareaPDF", m_pPdfarea);
    m_pPdfarea->set_cairo_debug(m_settings.getCairoDebug());

    //Get the label FileInfo.
    m_pLabelFileInfo = NULL;
    m_refGlade->get_widget("labelFileInfo", m_pLabelFileInfo);
    m_pLabelFileInfo->set_selectable(true);
    //m_pLabelFileInfo->set_line_wrap(true);

    //Get the Statusbar.
    m_pStatusbar = NULL;
    m_refGlade->get_widget("statusbar", m_pStatusbar);

    //Get the TreeView.
    m_pTreeView = NULL;
    m_refGlade->get_widget_derived("treeview1", m_pTreeView);

    //Connect activities and methods.

    m_refPreferences = Glib::RefPtr<Gtk::Action>::cast_static(m_refGlade->get_object ("preferences"));
    m_refPreferences->signal_activate().connect( sigc::mem_fun(this, &melangeWindow::on_action_preferences) );

    m_refAbout = Glib::RefPtr<Gtk::Action>::cast_static(m_refGlade->get_object ("about"));
    m_refAbout->signal_activate().connect( sigc::mem_fun(this, &melangeWindow::on_action_about) );

    m_refSystemInfo = Glib::RefPtr<Gtk::Action>::cast_static(m_refGlade->get_object ("systeminfo"));
    m_refSystemInfo->signal_activate().connect( sigc::mem_fun(this, &melangeWindow::on_action_system_info) );

    m_refDelete = Glib::RefPtr<Gtk::Action>::cast_static(m_refGlade->get_object ("delete"));
    m_refDelete->signal_activate().connect( sigc::mem_fun(m_pTreeView, &melangeTreeView::delete_selected) );

    m_refCut = Glib::RefPtr<Gtk::Action>::cast_static(m_refGlade->get_object ("cut"));
    m_refCut->signal_activate().connect( sigc::mem_fun(m_pTreeView, &melangeTreeView::cut_selected) );

    m_refCopy = Glib::RefPtr<Gtk::Action>::cast_static(m_refGlade->get_object ("copy"));
    m_refCopy->signal_activate().connect( sigc::mem_fun(m_pTreeView, &melangeTreeView::copy_selected) );

    m_refPaste = Glib::RefPtr<Gtk::Action>::cast_static(m_refGlade->get_object ("paste"));
    m_refPaste->signal_activate().connect( sigc::mem_fun(m_pTreeView, &melangeTreeView::paste) );

    m_refSelect_all = Glib::RefPtr<Gtk::Action>::cast_static(m_refGlade->get_object ("select_all"));
    m_refSelect_all->signal_activate().connect( sigc::bind<bool>( sigc::mem_fun(m_pTreeView, &melangeTreeView::select_all), true) );

    m_refUnselect_all = Glib::RefPtr<Gtk::Action>::cast_static(m_refGlade->get_object ("unselect_all"));
    m_refUnselect_all->signal_activate().connect( sigc::bind<bool>( sigc::mem_fun(m_pTreeView, &melangeTreeView::select_all), false) );

    m_refOpen = Glib::RefPtr<Gtk::Action>::cast_static(m_refGlade->get_object ("open"));
    m_refOpen->signal_activate().connect( sigc::mem_fun(this, &melangeWindow::on_action_open) );

    m_refSave = Glib::RefPtr<Gtk::Action>::cast_static(m_refGlade->get_object ("save"));
    m_refSave->signal_activate().connect( sigc::mem_fun(this, &melangeWindow::on_action_save) );

    m_refSave_as = Glib::RefPtr<Gtk::Action>::cast_static(m_refGlade->get_object ("save_as"));
    m_refSave_as->signal_activate().connect( sigc::mem_fun(this, &melangeWindow::on_action_save_as) );

    m_refQuit = Glib::RefPtr<Gtk::Action>::cast_static(m_refGlade->get_object ("quit"));
    m_refQuit->signal_activate().connect( sigc::mem_fun(this, &melangeWindow::on_action_quit) );

    m_refPage_next = Glib::RefPtr<Gtk::Action>::cast_static(m_refGlade->get_object ("page_next"));
    m_refPage_next->signal_activate().connect( sigc::mem_fun(this, &melangeWindow::on_page_next) );

    m_refPage_previous = Glib::RefPtr<Gtk::Action>::cast_static(m_refGlade->get_object ("page_previous"));
    m_refPage_previous->signal_activate().connect( sigc::mem_fun(this, &melangeWindow::on_page_previous) );

    m_refRotate_cw = Glib::RefPtr<Gtk::Action>::cast_static(m_refGlade->get_object ("rotate_cw"));
    m_refRotate_cw->signal_activate().connect( sigc::mem_fun(this, &melangeWindow::on_rotate_cw) );

    m_refRotate_ccw = Glib::RefPtr<Gtk::Action>::cast_static(m_refGlade->get_object ("rotate_ccw"));
    m_refRotate_ccw->signal_activate().connect( sigc::mem_fun(this, &melangeWindow::on_rotate_ccw) );

    m_refMove_page_up = Glib::RefPtr<Gtk::Action>::cast_static(m_refGlade->get_object ("move_page_up"));
    m_refMove_page_up->signal_activate().connect( sigc::mem_fun(m_pTreeView, &melangeTreeView::move_selected_up) );

    m_refMove_page_down = Glib::RefPtr<Gtk::Action>::cast_static(m_refGlade->get_object ("move_page_down"));
    m_refMove_page_down->signal_activate().connect( sigc::mem_fun(m_pTreeView, &melangeTreeView::move_selected_down) );

    m_refAppend = Glib::RefPtr<Gtk::Action>::cast_static(m_refGlade->get_object ("append"));
    m_refAppend->signal_activate().connect( sigc::mem_fun(this, &melangeWindow::on_action_append) );

    m_refInsert = Glib::RefPtr<Gtk::Action>::cast_static(m_refGlade->get_object ("insert"));
    m_refInsert->signal_activate().connect( sigc::mem_fun(this, &melangeWindow::on_action_insert) );

    m_refZoom_fit = Glib::RefPtr<Gtk::Action>::cast_static(m_refGlade->get_object ("zoom_fit"));
    m_refZoom_fit->signal_activate().connect( sigc::mem_fun(*m_pPdfarea, &melangeDrawingArea::draw_fitted_to_window) );

    m_refZoom_in = Glib::RefPtr<Gtk::Action>::cast_static(m_refGlade->get_object ("zoom_in"));
    m_refZoom_in->signal_activate().connect( sigc::mem_fun(*m_pPdfarea, &melangeDrawingArea::draw_zoom_in) );

    m_refZoom_out = Glib::RefPtr<Gtk::Action>::cast_static(m_refGlade->get_object ("zoom_out"));
    m_refZoom_out->signal_activate().connect( sigc::mem_fun(*m_pPdfarea, &melangeDrawingArea::draw_zoom_out) );

	m_pPdfarea->signal_idle.connect( sigc::mem_fun(*this, &melangeWindow::show_idle) );
	m_pTreeView->signal_idle.connect( sigc::mem_fun(*this, &melangeWindow::show_idle) );
	m_pTreeView->signal_selected.connect( sigc::mem_fun(*this, &melangeWindow::set_sensitive_on_selection) );
    m_pTreeView->signal_list_modified.connect( sigc::bind<bool>( sigc::mem_fun(*this, &melangeWindow::set_mainFile_is_modified), true) );
	m_pTreeView->signal_new.connect( sigc::mem_fun(*this, &melangeWindow::set_mainFileName) );

	// Connect own signals to callbacks. 
	
	m_pTreeView->signal_rotate_cw_request.connect( sigc::mem_fun(this, &melangeWindow::on_rotate_cw) );
	m_pTreeView->signal_rotate_ccw_request.connect( sigc::mem_fun(this, &melangeWindow::on_rotate_ccw) );
	m_pPdfarea->signal_rotate_cw_request.connect( sigc::mem_fun(this, &melangeWindow::on_rotate_cw) );
	m_pPdfarea->signal_rotate_ccw_request.connect( sigc::mem_fun(this, &melangeWindow::on_rotate_ccw) );

	m_pPdfarea->signal_page_previous.connect( sigc::mem_fun(this, &melangeWindow::on_page_previous) );
	m_pPdfarea->signal_page_next.connect( sigc::mem_fun(this, &melangeWindow::on_page_next) );

    //this->set_sensitive_on_selection(false);

    //Create the Tree model:
    m_refCopy_Buffer = melangeListStore::create();
    m_refTreeModel = melangeListStore::create();
    m_pTreeView->set_model(m_refTreeModel);

    m_refTreeModel->setIconSize(m_settings.getIconSize());

    m_refCursor = Gdk::Cursor::create(get_display(), Gdk::WATCH);

    this->show_all_children ();
}

/**
 * \brief Destructor
 */
melangeWindow::~melangeWindow()
{
}

/**
 * \brief Set the main file name.
 *
 * The main file is written when action "Save" is triggered.
 *
 * \param std::string name
 * \return void
 */
void melangeWindow::set_mainFileName(std::string name)
{
    m_MainFileName = name;
    set_title("pdfMelange: " + m_MainFileName);
    this->set_mainFile_is_modified(false);
}

/**
 * \brief Set the flag to indicate the model has been changed.
 *
 * \param bool is_modified
 * \return void
 */
void melangeWindow::set_mainFile_is_modified(bool is_modified)
{
    g_message("melangeWindow::set_mainFile_is_modified");

    m_refSave->set_sensitive(is_modified);
    m_is_MainFile_modified = is_modified;
}

/**
 * \brief Set the program name, complete path.
 *
 * \param const char* name
 * \return void
 */
void melangeWindow::set_programFileName(const char* name)
{
    g_message("melangeWindow::set_programFileName: %s", name);

    m_ProgramFileName = name;
}

/**
 * \brief Set the flag to indicate the selection has been changed.
 *
 * \param bool is_aktiv
 * \return void
 */
void melangeWindow::set_sensitive_on_selection(int number)
{
    g_message("melangeWindow::set_sensitive_on_selection");

	if (number){
		bool is_selected_first = m_pTreeView->is_selected_first_row();
		bool is_selected_last = m_pTreeView->is_selected_last_row();
		if (is_selected_first) {
			m_refPage_previous->set_sensitive(false);
			m_refMove_page_up->set_sensitive(false);
		}
		else {
			m_refPage_previous->set_sensitive(true);
			m_refMove_page_up->set_sensitive(true);
		}
		if (is_selected_last) {
			m_refPage_next->set_sensitive(false);
			m_refMove_page_down->set_sensitive(false);
		}
		else {
			m_refPage_next->set_sensitive(true);
			m_refMove_page_down->set_sensitive(true);
		}
		m_refCopy->set_sensitive(true);
		m_refCut->set_sensitive(true);
		m_refRotate_cw->set_sensitive(true);
		m_refRotate_ccw->set_sensitive(true);
		m_refDelete->set_sensitive(true);
	}
	else {
		m_refCopy->set_sensitive(false);
		m_refCut->set_sensitive(false);
		m_refPage_previous->set_sensitive(false);
		m_refPage_next->set_sensitive(false);
		m_refRotate_cw->set_sensitive(false);
		m_refRotate_ccw->set_sensitive(false);
		m_refMove_page_up->set_sensitive(false);
		m_refMove_page_down->set_sensitive(false);
		m_refDelete->set_sensitive(false);
	}

    if (number == 1) {
        set_pdf_preview();
    } else {
        clear_pdf_preview();
	}
}

/**
 * \brief Signal handler for action m_refAbout.
 */
void melangeWindow::on_action_about()
{
    g_message("melangeWindow::on_action_about");
    m_pStatusbar->push("About dialog ...");

    melangeAboutDialog dialog("About pdfMelange", *this, false);
    dialog.run();
    m_pStatusbar->push("");
}

/**
 * \brief Signal handler for action m_refPreferences.
 */
void melangeWindow::on_action_preferences()
{
    g_message("melangeWindow::on_action_preferences");
    m_pStatusbar->push("Preferences dialog ...");

    Glib::RefPtr<Gtk::Builder> builder;
    try
    {
        /** to create the ui-string header call "gtkbuilder2ustring ../src/melangePreferences.ui ../src/melangePreferences.ui-string.h" */
		#include "melangePreferences.ui-string.h"
        builder = Gtk::Builder::create_from_string(ui_builder);
    }
    catch (const Glib::FileError & ex)
    {
        std::cerr << ex.what() << std::endl;
        return;
    }

    melangePreferences* pDialog = NULL;
    builder->get_widget_derived("preferences", pDialog);

    m_settings.loadFromFile();
    pDialog->setConfig(m_settings);
    pDialog->set_transient_for(*this);

    switch(pDialog->run())
    {
    case(Gtk::RESPONSE_OK):
    {
        m_settings = pDialog->getConfig();
        m_settings.write();
        m_refTreeModel->setIconSize(m_settings.getIconSize());
        if (m_settings.getLogging()) melangeLogging::setLogging(m_settings.getLogging());
        g_message("logging was set to: %s", m_settings.getLogging() ? "true" : "false");
        g_message("Iconsize          : %i", m_settings.getIconSize());
        g_message("Page mode         : %s", m_settings.getPageMode());
        g_message("Page layout       : %s", m_settings.getPageLayout());
        if (!m_settings.getLogging()) melangeLogging::setLogging(m_settings.getLogging());
        break;
    }
    case(Gtk::RESPONSE_CANCEL):
    {
        break;
    }
    default:
    {
        break;
    }
    }

    delete pDialog;

    m_pStatusbar->push("");
}

/**
 * \brief Signal handler for action m_refSystemInfo.
 */
void melangeWindow::on_action_system_info()
{
    g_message("melangeWindow::on_action_system_info");
    m_pStatusbar->push("System Info dialog ...");

    Glib::ustring infoText;
    gchar* cstr;

    infoText = "<small>";
    infoText += "<b><tt>Program path: </tt></b>" + m_ProgramFileName + "\n";
    infoText += "<b><tt>Logfile     : </tt></b>" + m_LogFileName + "\n";
    infoText += "<b><tt>Configfile  : </tt></b>" + Glib::ustring(m_settings.getFileName()) + "\n";

#ifdef G_OS_UNIX
    struct utsname u_name;
    uname(&u_name);
    infoText += "\n";
    infoText += cstr = g_strdup_printf("<b><tt>OS Name     :</tt></b> %s\n", u_name.sysname);
    g_free(cstr);
    infoText += cstr = g_strdup_printf("<b><tt>OS Release  :</tt></b> %s\n", u_name.release);
    g_free(cstr);
    infoText += cstr = g_strdup_printf("<b><tt>OS Version  :</tt></b> %s\n", u_name.version);
    g_free(cstr);
    infoText += "\n";
    infoText += cstr = g_strdup_printf("<b><tt>Machine     :</tt></b> %s\n", u_name.machine);
    g_free(cstr);
#endif

    infoText += "\n<b><tt>Libraries</tt></b>\n";
    infoText += cstr = g_strdup_printf("     glib-%i.%i.%i\n", GLIB_MAJOR_VERSION, GLIB_MINOR_VERSION, GLIB_MICRO_VERSION);
    g_free(cstr);
    infoText += cstr = g_strdup_printf("     gtkmm-%i.%i.%i\n", GTKMM_MAJOR_VERSION, GTKMM_MINOR_VERSION, GTKMM_MICRO_VERSION);
    g_free(cstr);
    infoText += cstr = g_strdup_printf("     gtk+-%i.%i.%i\n", GTK_MAJOR_VERSION, GTK_MINOR_VERSION, GTK_MICRO_VERSION);
    g_free(cstr);
    infoText += cstr = g_strdup_printf("     cairomm-%i.%i.%i\n", CAIROMM_MAJOR_VERSION, CAIROMM_MINOR_VERSION, CAIROMM_MICRO_VERSION);
    g_free(cstr);
    infoText += cstr = g_strdup_printf("     cairo-%i.%i.%i\n", CAIRO_VERSION_MAJOR, CAIRO_VERSION_MINOR, CAIRO_VERSION_MICRO);
    g_free(cstr);
    infoText += cstr = g_strdup_printf("     poppler-%i.%i.%i\n", POPPLER_MAJOR_VERSION, POPPLER_MINOR_VERSION, POPPLER_MICRO_VERSION);
    g_free(cstr);
    //infoText += cstr = g_strdup_printf("     python-%i.%i.%i", PY_MAJOR_VERSION, PY_MINOR_VERSION, PY_MICRO_VERSION);
    //g_free(cstr);
    infoText += "</small>";

    Gtk::MessageDialog dialog(*this, "", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK);
    dialog.set_title("System information");
    dialog.set_secondary_text(infoText, true);
    dialog.run();
    m_pStatusbar->push("");
}

/**
 * \brief Signal handler for action m_refQuit.
 */
void melangeWindow::on_action_quit()
{
    g_message("melangeWindow::on_action_quit");

    if (m_is_MainFile_modified)
    {
        //todo: dialog that verifies if quit without save.
    }
    //list_the_tree();
    hide ();
}

/**
 * \brief Signal handler for action m_refSave.
 */
void melangeWindow::on_action_save()
{
    g_message("melangeWindow::on_action_save");

    clear_pdf_preview();

    if (m_MainFileName.size() == 0)
    {
        this->on_action_save_as();
        return;
    }

    if (Glib::file_test(m_MainFileName, Glib::FILE_TEST_EXISTS))
    {
        Gtk::MessageDialog messagedialog(*this,
                                         _("File already exists. Replace the old one?"),
                                         false /* use_markup */,
                                         Gtk::MESSAGE_WARNING,
                                         Gtk::BUTTONS_NONE);
        messagedialog.add_button(Gtk::Stock::SAVE_AS, Gtk::RESPONSE_APPLY);
        messagedialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
        messagedialog.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

        int result = messagedialog.run();

        //Handle the response:
        switch(result)
        {
		    case(Gtk::RESPONSE_OK):
		    {
		        this->write_document();
		        this->set_mainFile_is_modified(false);
		        break;
		    }
		    case(Gtk::RESPONSE_CANCEL):
		    {
		        break;
		    }
		    case(Gtk::RESPONSE_APPLY):
		    {
		        on_action_save_as();
		        break;
		    }
		    default:
		    {
		        g_message("   Unexpected button clicked.");
		        break;
		    }
        }
    }
    else
    {
        this->write_document();
        this->set_mainFile_is_modified(false);
    }
}

/**
 * \brief Execute a write to main file name.
 */
void melangeWindow::write_document()
{
    g_message("melangeWindow::write_document");

    try
    {
		show_idle ();
        this->m_refTreeModel->write_pdf_document(m_MainFileName.c_str(), m_settings);
		show_idle ( false );
        m_pStatusbar->push(_("saved file ") + Glib::ustring("\"") + m_MainFileName + Glib::ustring("\""));
    }
    catch (char* msg)
    {
        g_message("melangeWindow::write_document: catched error <%s>", msg);
		show_idle ( false );

        char* fullMessage = g_strdup_printf("Error while writing output file: %s\n"
                                            "Cached error message: <%s>\n"
                                            "Check log file: <%s> for detailed information."
                                            ,m_MainFileName.c_str(), msg, m_LogFileName.c_str());
        free(msg);
        Gtk::MessageDialog messagedialog(*this,
                                         fullMessage,
                                         false, /* use_markup */
                                         Gtk::MESSAGE_ERROR,
                                         Gtk::BUTTONS_CLOSE);
        messagedialog.run();
        g_free(fullMessage);
    }
}

/**
 * \brief Signal handler for action m_refSave_as.
 */
void melangeWindow::on_action_save_as()
{
    g_message("melangeWindow::on_action_save_as");

    clear_pdf_preview();

    melangeSaveAsDialog filedialog(_("Save file"), this);
    filedialog.set_filename(m_MainFileName);
	filedialog.setConfig(m_settings);

    // Loop the filedialog while messagedialog "File already exists. Replace the old one?" responses OK.
    bool doLoop = true;
    while (doLoop)
    {
        doLoop = false;

        //Show the filedialog and wait for a user response:
        int result = filedialog.run();

        //Handle the response:
        switch(result)
        {
        case(Gtk::RESPONSE_OK):
        {
            g_message("   File selected: %s", filedialog.get_filename().c_str());
            g_message("   URI  selected: %s", filedialog.get_uri().c_str());

            if (Glib::file_test(filedialog.get_filename(), Glib::FILE_TEST_EXISTS))
            {
                Gtk::MessageDialog messagedialog(*this,
                                                 _("File already exists. Replace the old one?"),
                                                 false /* use_markup */,
                                                 Gtk::MESSAGE_WARNING,
                                                 Gtk::BUTTONS_NONE);
                messagedialog.add_button(Gtk::Stock::SAVE_AS, Gtk::RESPONSE_APPLY);
                messagedialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
                messagedialog.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

                int result = messagedialog.run();

                //Handle the response:
                switch(result)
                {
		            case(Gtk::RESPONSE_OK):
		            {
						m_settings = filedialog.getConfig();
		                this->set_mainFileName(filedialog.get_filename());
		                this->write_document();
		                this->set_mainFile_is_modified(false);
		                break;
		            }
		            case(Gtk::RESPONSE_CANCEL):
		            {
		                doLoop = true;
		                break;
		            }
		            case(Gtk::RESPONSE_APPLY):
		            {
		                on_action_save_as();
		                break;
		            }
		            default:
		            {
		                g_message("   Unexpected button clicked.");
		                break;
		            }
                }
            }
            else
            {
                //Glib::ustring filename = filedialog.get_filename();
                std::string filename = filedialog.get_filename();
                if ( ! (Glib::str_has_suffix(filename, ".pdf") || Glib::str_has_suffix(filename, ".PDF")) ) filename += ".pdf";
                this->set_mainFileName(filename);
                this->write_document();
                this->set_mainFile_is_modified(false);
            }
            break;
        }
        case(Gtk::RESPONSE_CANCEL):
        {
            break;
        }
        default:
        {
            g_message("   Unexpected button clicked.");
            break;
        }
        }
    }
}

/**
 * \brief Getter for a pdf filename.
 *
 * \param std::string &filename
 * \param Glib::ustring &uri
 * \return true on success, else false.
 */
bool melangeWindow::file_open_dialog(std::string &filename, Glib::ustring &uri)
{
    g_message("melangeWindow::file_open_dialog");

    Gtk::FileChooserDialog dialog(_("Please choose a file"), Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);
	
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
		    filename = dialog.get_filename();
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
}


/**
 * \brief Signal handler for action m_refOpen.
 */
void melangeWindow::on_action_open()
{
    g_message("melangeWindow::on_action_open");

    std::string filename;
    Glib::ustring uri;

    bool response = file_open_dialog(filename, uri);
    if (response)
    {
        set_mainFileName(filename);
		show_idle( true );
		m_pTreeView->clear();
		m_pTreeView->append(uri);
		show_idle( false );
    }
}

/**
 * \brief Clear the drawingareaPDF.
 */
void melangeWindow::clear_pdf_preview()
{
    g_message("melangeWindow::clear_pdf_preview");
    m_pPdfarea->unset_page();
    m_pLabelFileInfo->set_markup("");
}

/**
 * \brief Draw the PDF inside the drawingareaPDF.
 *
 * \return void.
 */
void melangeWindow::set_pdf_preview()
{
    g_message("melangeWindow::set_pdf_preview");
	show_idle ( true );  // this is deactivated by m_pPdfarea->signal_active

    //get the first selected row
    std::vector<Gtk::TreeModel::Path> pathlist = m_pTreeView->get_selection()->get_selected_rows();
    Gtk::TreeModel::iterator iter = m_refTreeModel->get_iter(pathlist[0]);
    Gtk::TreeModel::Row row = *iter;

    int page				= (int)row[m_refTreeModel->m_Columns.page]-1;
    int angle				= (int)row[m_refTreeModel->m_Columns.angle];
    Glib::ustring uri		= row[m_refTreeModel->m_Columns.uri];
    Glib::ustring password  = row[m_refTreeModel->m_Columns.password];
    PopplerPermissions perm = row[m_refTreeModel->m_Columns.permissions];
    bool encrypted          = row[m_refTreeModel->m_Columns.encrypted];

    PopplerDocument* pDoc = NULL;
    PopplerPage* pPage = NULL;
    GError* err = NULL;

    pDoc = poppler_document_new_from_file(uri.c_str(), password.c_str(), &err);
    if (pDoc) {
        pPage = poppler_document_get_page(pDoc, page);
        m_pPdfarea->unset_page();
        m_pPdfarea->set_page(pDoc, pPage, angle);

        Glib::ustring infoText;
        gchar* cstr;
        infoText = "<small>";
        infoText +=                        "<b><tt>PDF file name    :</tt></b>" + Glib::filename_display_name(Glib::filename_from_uri(uri)) + "\n";
        infoText += Glib::ustring::compose("<b><tt>Page             :</tt></b> %1 of %2\n", page+1, poppler_document_get_n_pages(pDoc));
        infoText += Glib::ustring::compose("<b><tt>Rotation         :</tt></b> %1\n", angle);
        if (cstr = poppler_document_get_title(pDoc)) {
            infoText += Glib::ustring::compose("<b><tt>Title            :</tt></b> %1\n", cstr);
            g_free(cstr);
        }
        if (cstr = poppler_document_get_author(pDoc)) {
            infoText += Glib::ustring::compose("<b><tt>Author           :</tt></b> %1\n", cstr);
            g_free(cstr);
        }
        if (cstr = poppler_document_get_subject(pDoc)) {
            infoText += Glib::ustring::compose("<b><tt>Subject          :</tt></b> %1\n", cstr);
            g_free(cstr);
        }
        if (cstr = poppler_document_get_keywords(pDoc)) {
            infoText += Glib::ustring::compose("<b><tt>Keywords         :</tt></b> %1\n", cstr);
            g_free(cstr);
        }
        if (cstr = poppler_document_get_creator(pDoc)) {
            infoText += Glib::ustring::compose("<b><tt>Creator          :</tt></b> %1\n", cstr);
            g_free(cstr);
        }
        if (cstr = poppler_document_get_producer(pDoc)) {
            infoText += Glib::ustring::compose("<b><tt>Producer         :</tt></b> %1\n", cstr);
            g_free(cstr);
        }

        gint64 time_t;
        GDateTime *gtime;
        gchar *time_str;

        time_t = poppler_document_get_creation_date(pDoc);
        if (time_t > 0) {
            gtime = g_date_time_new_from_unix_local(time_t);
            cstr = g_date_time_format(gtime, "%F - %T");
            infoText += Glib::ustring::compose("<b><tt>Creation Date    :</tt></b> %1\n", cstr);
            g_date_time_unref(gtime);
            g_free(cstr);
        }

        time_t = poppler_document_get_modification_date(pDoc);
        if (time_t > 0) {
            gtime = g_date_time_new_from_unix_local(time_t);
            cstr = g_date_time_format(gtime, "%F - %T");
            infoText += Glib::ustring::compose("<b><tt>Modification Date:</tt></b> %1\n", cstr);
            g_date_time_unref(gtime);
            g_free(cstr);
        }

        double width, height;
        poppler_page_get_size(pPage, &width, &height);
        infoText += Glib::ustring::compose("<b><tt>Page dimension   :</tt></b> %1 x %2\n", width, height);

        infoText += Glib::ustring::compose("<b><tt>Encryption       :</tt></b> %1\n", encrypted ? "yes" : "no");
        infoText +=                        "<b><tt>Permissions      :</tt></b>";
        if ((perm & POPPLER_PERMISSIONS_FULL) == POPPLER_PERMISSIONS_FULL) {
            infoText += " full\n";
        } else {
            infoText += "\n";
            infoText += "<span size=\"xx-small\" font=\"Monospace\" >";
            infoText += Glib::ustring::compose("                 to print: %1\n", (perm & POPPLER_PERMISSIONS_OK_TO_PRINT) ? "yes" : "<b>no</b>");
            infoText += Glib::ustring::compose("                to modify: %1\n", (perm & POPPLER_PERMISSIONS_OK_TO_MODIFY) ? "yes" : "<b>no</b>");
            infoText += Glib::ustring::compose("                  to copy: %1\n", (perm & POPPLER_PERMISSIONS_OK_TO_COPY) ? "yes" : "<b>no</b>");
            infoText += Glib::ustring::compose("             to add notes: %1\n", (perm & POPPLER_PERMISSIONS_OK_TO_ADD_NOTES) ? "yes" : "<b>no</b>");
            infoText += Glib::ustring::compose("             to fill form: %1\n", (perm & POPPLER_PERMISSIONS_OK_TO_FILL_FORM) ? "yes" : "<b>no</b>");
            infoText += Glib::ustring::compose("      to extract contents: %1\n", (perm & POPPLER_PERMISSIONS_OK_TO_EXTRACT_CONTENTS) ? "yes" : "<b>no</b>");
            infoText += Glib::ustring::compose("              to assemble: %1\n", (perm & POPPLER_PERMISSIONS_OK_TO_ASSEMBLE) ? "yes" : "<b>no</b>");
            infoText += Glib::ustring::compose(" to print high resolution: %1\n", (perm & POPPLER_PERMISSIONS_OK_TO_PRINT_HIGH_RESOLUTION) ? "yes" : "<b>no</b>");
            infoText += "</span>";
        }

        infoText += "</small>";
        m_pLabelFileInfo->set_markup(infoText);
//        m_pLabelFileInfo->select_region(0,0);

        infoText.erase();
        infoText += cstr = g_strdup_printf(_("file \"%s\" page %i"), Glib::filename_display_basename(Glib::filename_from_uri(uri)).c_str(), page+1);
        g_free(cstr);
        m_pStatusbar->push(infoText);
    }

}

/**
 *  \brief List all elements of the main TreeModel to std::out.
 */
void melangeWindow::list_the_tree()
{
    m_refTreeModel->print();
};  

/**
 * \brief Show curser as idle.
 * 
 */
void melangeWindow::show_idle(bool isIdle )
{
    g_message("melangeWindow::show_idle %s", isIdle ? "true" : "false");

	if (isIdle) {
		static Glib::RefPtr<Gdk::Cursor> wC = Gdk::Cursor::create( get_display(), Gdk::WATCH);
		m_refWindow->set_cursor(wC);
		while( Gtk::Main::events_pending() ) Gtk::Main::iteration();
	}
    else
    {
		m_refWindow->set_cursor();
    }
}

/**
 * \brief Signal handler for action m_refRotate_cw.
 */
void melangeWindow::on_rotate_cw()
{
    g_message("melangeWindow::on_rotate_cw");

	show_idle ( true );
	m_pTreeView->rotate_selected_cw();
	m_pPdfarea->rotate_cw();
}

/**
 * \brief Signal handler for action m_refRotate_ccw.
 */
void melangeWindow::on_rotate_ccw()
{
    g_message("melangeWindow::on_rotate_ccw");

	show_idle ( true );
	m_pTreeView->rotate_selected_ccw();
	m_pPdfarea->rotate_ccw();
}

/**
 * \brief Signal handler for once move forward.
 */
void melangeWindow::on_page_next()
{
    g_message("melangeWindow::on_page_next");

	m_pTreeView->select_next();
}

/**
 * \brief Signal handler for once move backward.
 */
void melangeWindow::on_page_previous()
{
    g_message("melangeWindow::on_page_previous");

	m_pTreeView->select_previous();
}

/**
 * \brief Signal handler for action m_refInsert.
 */
void melangeWindow::on_action_insert()
{
    g_message("melangeWindow::on_action_insert");

    std::string filename;
    Glib::ustring uri;
    if (this->file_open_dialog(filename, uri))
    {
		show_idle ( true );
        m_pTreeView->insert(uri);
		show_idle ( false );
    }

}

/**
 * \brief Append the pdf file represented in uri.
 */
void melangeWindow::append_document(Glib::ustring uri)
{
    g_message("melangeWindow::append_document");

	show_idle ( true );
    m_pTreeView->append(uri);
	show_idle ( false );
}

/**
 * \brief Signal handler for action m_refAppend.
 */
void melangeWindow::on_action_append()
{
    g_message("melangeWindow::on_action_append");

    std::string filename;
    Glib::ustring uri;

    if (this->file_open_dialog(filename, uri))
    {
		show_idle ( true );
        m_pTreeView->append(uri);
		show_idle ( false );
    }
}
