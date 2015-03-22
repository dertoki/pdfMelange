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
#include "melangeKeyFile.h"
#include "melangeDrawingArea.h"
#include <cairomm/context.h>
#include <cairomm/matrix.h>
#include <poppler/glib/poppler.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#ifndef M_PI
    #define M_PI    3.14159265358979323846f
#endif

/**
 * \brief Constructor
 */
melangeDrawingArea::melangeDrawingArea()
{
    init();
}

/**
 * \brief Constructor for Gtk::Builder
 *
 * 	  m_pPdfarea = NULL;
 *    m_refGlade->get_widget_derived("drawingareaPDF", m_pPdfarea);
 *
 */
melangeDrawingArea::melangeDrawingArea(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade)
    : Gtk::DrawingArea(cobject)
{
    init();
}

/**
 * \brief Destructor
 */
melangeDrawingArea::~melangeDrawingArea()
{
	
}

/**
 * \brief Initialization function for the constructors.
 *
 * Set initial values, handling of signals and events.
 */
void melangeDrawingArea::init()
{
    this->m_pdfRotation = 0;
    this->m_pPage = NULL;
    this->m_pDoc = NULL;

	this->m_cairo_debug = false;

    this->m_button1 = false;
    this->m_x = 0;
    this->m_y = 0;
    this->m_dx = 0;
    this->m_dy = 0;
    this->m_zoom = 1;
    this->m_pdfScale = 1;
    this->m_mtx = Cairo::translation_matrix (0, 0);
    this->m_mtx_zero = Cairo::translation_matrix (0, 0);

    add_events(Gdk::SCROLL_MASK);
    add_events(Gdk::BUTTON_PRESS_MASK);
    add_events(Gdk::BUTTON_RELEASE_MASK);
    add_events(Gdk::BUTTON_MOTION_MASK);
    signal_size_allocate().connect(sigc::mem_fun(*this, &melangeDrawingArea::on_my_size_allocate));
    signal_scroll_event().connect(sigc::mem_fun(*this, &melangeDrawingArea::on_event_scroll));
    signal_button_press_event().connect(sigc::mem_fun(*this, &melangeDrawingArea::on_button_press_event) );
    signal_button_release_event().connect(sigc::mem_fun(*this, &melangeDrawingArea::on_button_release_event) );
    signal_motion_notify_event().connect(sigc::mem_fun(*this, &melangeDrawingArea::on_motion_notify_event) );
}

/**
 * \brief Set a pdf page and redraw.
 *
 * \param PopplerDocument*
 * \param PopplerPage*
 * \param int angle (rotation)
 * \return void
 */
void melangeDrawingArea::set_page(PopplerDocument* pDoc, PopplerPage* pPage, int angle)
{
    this->m_pDoc = pDoc;
    this->m_pPage = pPage;
    this->m_pdfRotation = angle;
    this->m_mtx_zero = calculate_init_matrix();
    this->m_mtx = m_mtx_zero * Cairo::identity_matrix();

    this->queue_draw ();
}

/**
 * \brief Redraw without a pdf page and clean up.
 */
void melangeDrawingArea::unset_page()
{
    if (this->m_pPage) g_object_unref(this->m_pPage); // this creates an error with gtk 3.10
    if (this->m_pDoc) g_object_unref(this->m_pDoc);
    this->m_pPage = NULL;
    this->m_pDoc = NULL;

    this->queue_draw ();
}

/**
 * \brief Override the default handler for the signal signal_draw().
 *
 * \param Cairo::RefPtr<Cairo::Context>& m_refCr
 * \return true
 */
bool melangeDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& m_refCr)
{
    Gtk::Allocation allocation = get_allocation();
    m_width = allocation.get_width();
    m_height = allocation.get_height();

    /// set background color
    m_refCr->set_source_rgb(1.0, 1.0, 1.0);
    m_refCr->paint();

 	this->m_mtx_virgin = m_refCr->get_matrix();
	/// draw into context
    if (m_pPage) {
        m_refCr->set_matrix ( m_mtx * m_refCr->get_matrix() );
        draw_page_frame(m_refCr);
		poppler_page_render(m_pPage, m_refCr->cobj());
    }
    if (m_cairo_debug) _test_draw( m_refCr );

    return true;
}

/**
 * \brief Draws a rectangle that surrounds the pdf page.
 *
 * \param Cairo::RefPtr<Cairo::Context>& m_refCr
 * \return void
 */
void melangeDrawingArea::draw_page_frame(const Cairo::RefPtr<Cairo::Context>& m_refCr)
{
    double pdfwidth, pdfheight;
    poppler_page_get_size(m_pPage, &pdfwidth, &pdfheight);

    m_refCr->move_to (0,0);
    m_refCr->line_to (pdfwidth, 0);
    m_refCr->line_to (pdfwidth, pdfheight);
    m_refCr->line_to (0, pdfheight);
    m_refCr->line_to (0, 0);

    m_refCr->set_source_rgb (0.0,0.0,0.0);
    m_refCr->set_line_width (1.0);
    m_refCr->stroke ();
}

/**
 * \brief Redraws, fitted to the window size.
 */
void melangeDrawingArea::draw_fitted_to_window()
{
    if (m_pPage) {
        this->m_mtx_zero = calculate_init_matrix();
        this->m_mtx = m_mtx_zero * Cairo::translation_matrix (0, 0);
        this->queue_draw ();
    }
}

/**
 * \brief Redraws zoomed in, the base point is the center of the window.
 */
void melangeDrawingArea::draw_zoom_in()
{
    if (m_pPage) {
        m_zoom = m_zoom_plus;
        m_mtx = m_mtx * calculate_zoom_matrix(m_width/2, m_height/2, m_zoom);
        m_pdfScale *= m_zoom;

        queue_draw(); // Queuing an expose event to the main loop..
    }
}

/**
 * \brief Redraw zoom out, the base point is the center of the window.
 */
void melangeDrawingArea::draw_zoom_out()
{
    if (m_pPage) {
        m_zoom = m_zoom_minus;
        m_mtx = m_mtx * calculate_zoom_matrix(m_width/2, m_height/2, m_zoom);
        m_pdfScale *= m_zoom;

        queue_draw(); // Queuing an expose event to the main loop..
    }
}

/**
 * \brief Calculates a transformation matrix for a zoom.
 *
 * Calculate a transformation matrix to scale at a particular point:
 *      Therefor we need a new transformation matrix that contains an affine transformation sequence as follows:
 *        1. translate point to center,
 *        2. scale at center,
 *        3. translate from center back to point.
 *      The affine transformation has to be done with a matrix multiplication in the order 1.,2.,3..
 *
 * \param horizontal position
 * \param vertical position
 * \param zoom factor
 * \return Cairo::Matrix mtx
 */
Cairo::Matrix melangeDrawingArea::calculate_zoom_matrix(double x, double y, double zoom)
{
    Cairo::Matrix mtx = Cairo::translation_matrix(-x, -y);
    mtx = mtx * Cairo::scaling_matrix (zoom, zoom);
    mtx = mtx * Cairo::translation_matrix (x, y);

    return mtx;
}

/**
 * \brief Creates a initial transformation matrix.
 *
 * Calculates a transformation matrix for the pdf page to fit inside the window.
 *
 * \return Cairo::Matrix mtx
 */
Cairo::Matrix melangeDrawingArea::calculate_init_matrix()
{
    double xScale, yScale, pdfScale;
    double xShift, yShift;
    double pdfwidth, pdfheight;
    poppler_page_get_size(m_pPage, &pdfwidth, &pdfheight);

    switch (m_pdfRotation) {
    case 0:
        xScale = m_width/pdfwidth;
        yScale = m_height/pdfheight;
        if (xScale < yScale) {
            pdfScale = xScale;
            xShift = 0;
            yShift = 0.5*(m_height - pdfScale*pdfheight);
        }
        else {
            pdfScale = yScale;
            xShift = 0.5*(m_width - pdfScale*pdfwidth);
            yShift = 0;
        }
        break;
    case 90:
        xScale = m_width/pdfheight;
        yScale = m_height/pdfwidth;
        if (xScale < yScale) {
            pdfScale = xScale;
            xShift = m_width;
            yShift = 0.5*(m_height - pdfScale*pdfwidth);
        }
        else {
            pdfScale = yScale;
            xShift = m_width - 0.5*(m_width - pdfScale*pdfheight);
            yShift = 0;
        }
        break;
    case 180:
        xScale = m_width/pdfwidth;
        yScale = m_height/pdfheight;
        if (xScale < yScale) {
            pdfScale = xScale;
            xShift = m_width;
            yShift = m_height - 0.5*(m_height - pdfScale*pdfheight);
        }
        else {
            pdfScale = yScale;
            xShift = m_width - 0.5*(m_width - pdfScale*pdfwidth);
            yShift = m_height;
        }
        break;
    case 270:
        xScale = m_width/pdfheight;
        yScale = m_height/pdfwidth;
        if (xScale < yScale) {
            pdfScale = xScale;
            xShift = 0;
            yShift = m_height - 0.5*(m_height - pdfScale*pdfwidth);
        }
        else {
            pdfScale = yScale;
            xShift = 0.5*(m_width - pdfScale*pdfheight);
            yShift = m_height;
        }
        break;
    default:
        ;
    }

    Cairo::Matrix mtx = Cairo::translation_matrix(xShift, yShift);
    mtx.rotate(m_pdfRotation*M_PI/180.0);
    mtx.scale(pdfScale, pdfScale);

    m_pdfScale = pdfScale;

    return mtx;
}

/**
 * \brief Draws some line and text elements.
 *
 *  For testing purposes.
 *
 * \param m_refCr: Cairo::RefPtr<Cairo::Context>&
 * \return void
 */
void melangeDrawingArea::_test_draw(const Cairo::RefPtr<Cairo::Context>& m_refCr)
{
    // dimension of the infobox.
    const double FONT_SIZE = 14.0;
	int bw, bh;
    if (m_pPage)
    {
        bw =  23*FONT_SIZE;
        bh = 8.0*FONT_SIZE;
    } else {
		bw =  23*FONT_SIZE;
		bh = 5.0*FONT_SIZE;
	}

    struct cpoint
    {
        int x;
        int y;
    };
    std::vector<cpoint> p = {
        {                0,  0               },
        {          m_width,  0               },
        {          m_width,  m_height        },
        {                0,  m_height        },
        { m_width/2 - bw/2,  m_height/2 - bh/2 },
        { m_width/2 + bw/2,  m_height/2 - bh/2 },
        { m_width/2 + bw/2,  m_height/2 + bh/2 },
        { m_width/2 - bw/2,  m_height/2 + bh/2 }
    };

	// set the virgin transformation matrix.
	m_refCr->set_matrix (m_mtx_virgin);

    // draw red lines out from the center of the window
    m_refCr->set_source_rgb(0.8, 0.0, 0.0);
    m_refCr->set_line_width(1.0);

    m_refCr->move_to(p[0].x, p[0].y);
    m_refCr->line_to(p[4].x, p[4].y);

    m_refCr->move_to(p[1].x, p[1].y);
    m_refCr->line_to(p[5].x, p[5].y);

    m_refCr->move_to(p[2].x, p[2].y);
    m_refCr->line_to(p[6].x, p[6].y);

    m_refCr->move_to(p[3].x, p[3].y);
    m_refCr->line_to(p[7].x, p[7].y);

    m_refCr->move_to(p[4].x, p[4].y);
    m_refCr->line_to(p[5].x, p[5].y);
    m_refCr->line_to(p[6].x, p[6].y);
    m_refCr->line_to(p[7].x, p[7].y);
    m_refCr->line_to(p[4].x, p[4].y);

    m_refCr->stroke();

    // draw some text
    std::stringstream t1, t2, t3, t4;
    t1.width(8); t1.precision(2); t1.setf(std::ios::fixed);
    t2.width(8); t2.precision(2); t2.setf(std::ios::fixed);
    t3.width(8); t3.precision(2); t3.setf(std::ios::fixed);
    t4.width(8); t4.precision(2); t4.setf(std::ios::fixed);
    
    Cairo::RefPtr<Cairo::ToyFontFace> font =
        Cairo::ToyFontFace::create("Bitstream Charter",
                                   Cairo::FONT_SLANT_ITALIC,
                                   Cairo::FONT_WEIGHT_BOLD);
    m_refCr->set_font_face(font);
    m_refCr->set_font_size(FONT_SIZE);

    // draw the text
    t1 << "cairo context:";
    m_refCr->move_to(p[4].x + FONT_SIZE/2, p[4].y + FONT_SIZE);
    m_refCr->show_text(t1.str());

    t2 << "width = " << m_width  << ", height = " << m_height;
    m_refCr->move_to(p[4].x + FONT_SIZE, p[4].y + 2*FONT_SIZE);
    m_refCr->show_text(t2.str());

    Cairo::Matrix mtx = m_mtx * m_refCr->get_matrix();

	t3 << "matrix xx=" <<  mtx.xx << " xy=" << mtx.xy << " x0="<< mtx.x0;
    m_refCr->move_to(p[4].x + FONT_SIZE, p[4].y + 3*FONT_SIZE);
    m_refCr->show_text(t3.str());

	t4 << "            yx=" <<  mtx.yx << " yy=" << mtx.yy << " y0="<< mtx.y0;
    m_refCr->move_to(p[4].x + FONT_SIZE, p[4].y + 4*FONT_SIZE);
    m_refCr->show_text(t4.str());

    if (m_pPage)
    {
        // calculate dimensions of the PDF page.
        double pdfwidth, pdfheight;
        poppler_page_get_size(m_pPage, &pdfwidth, &pdfheight);

        std::stringstream t3, t4, t5;

        t3 << "PDF dimensions";
        m_refCr->move_to(p[4].x + FONT_SIZE/2, p[4].y + 5*FONT_SIZE);
        m_refCr->show_text(t3.str());

        t4 << "width = " << pdfwidth  << ", height = " << pdfheight;
        m_refCr->move_to(p[4].x + FONT_SIZE, p[4].y + 6*FONT_SIZE);
        m_refCr->show_text(t4.str());

        t5 << "scale = " <<  m_pdfScale;
        m_refCr->move_to(p[4].x + FONT_SIZE, p[4].y + 7*FONT_SIZE);
        m_refCr->show_text(t5.str());
    }
}

/**
 * \brief handles size allocation signals.
 *
 * Bound to melangeDrawingArea with
 *    this->signal_size_allocate().connect(sigc::mem_fun(*this, &melangeDrawingArea::on_my_size_allocate));
 *
 * \param allocation: the new area.
 * \return void
 */
void melangeDrawingArea::on_my_size_allocate(Gtk::Allocation& allocation)
{
    m_width = allocation.get_width();
    m_height = allocation.get_height();

    // do some calculation
    if (m_pPage) {
        m_mtx_zero = calculate_init_matrix();
    }
}

/**
 * \brief handles scroll events.
 *
 * Get the mouse position, modify the zoom member variable and recalculate the transformation matrix.
 *
 * \param event
 * \return true if handled, else false.
 */
bool melangeDrawingArea::on_event_scroll(GdkEventScroll *e)
{
    m_x = e->x;
    m_y = e->y;
    if (e->direction == GDK_SCROLL_UP) {
        m_zoom = 1.1;
    }
    if (e->direction == GDK_SCROLL_DOWN) {
        m_zoom = 0.9;
    }

    m_mtx = m_mtx * calculate_zoom_matrix(m_x, m_y, m_zoom);
    m_pdfScale *= m_zoom;

    queue_draw(); // Queuing an expose event to the main loop..
    return true;
}

/**
 * \brief handle the button press event.
 *
 *  Set the marker for the left button and store the mouse position.
 *
 * \param event
 * \return true if handled, else false.
 */
bool melangeDrawingArea::on_button_press_event(GdkEventButton* event)
{
    if( (event->type == GDK_BUTTON_PRESS) && (event->button == 1) ) {
        m_button1 = true;
        m_x0 = event->x;
        m_y0 = event->y;
        return true; //It has been handled.
    }
    else
        return false;
}

/**
 * \brief handle the button release event.
 *
 *  Unset the marker for the left button.
 *
 * \param event
 * \return true if handled, else false.
 */
bool melangeDrawingArea::on_button_release_event(GdkEventButton* event)
{
    if( (event->type == GDK_BUTTON_RELEASE) && (event->button == 1) ) {
        m_button1 = false;
        return true; //It has been handled.
    }
    else
        return false;
}

/**
 * \brief handle mouse movement.
 *
 *  When the left button is pressed,
 *    1. calculate the move distance to the previous call,
 *    2. modify the transformation matrix,
 *    3. queue a redraw.
 *
 * \param event
 * \return true if handled, else false.
 */
bool melangeDrawingArea::on_motion_notify_event(GdkEventMotion* event)
{
    if( (event->type == GDK_MOTION_NOTIFY) && m_button1 ) {
        m_x = event->x;
        m_y = event->y;
        m_dx = m_x - m_x0;
        m_dy = m_y - m_y0;
        m_mtx = m_mtx * Cairo::translation_matrix (m_dx, m_dy);

        queue_draw(); // Queuing an expose event to the main loop..
        m_x0 = m_x;
        m_y0 = m_y;
        return true; //It has been handled.
    }
    else
        return false;
}

/**
 * \brief setter for debugging.
 *
 * \param debug
 */
void melangeDrawingArea::set_cairo_debug(bool debug)
{
    m_cairo_debug = debug;
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~| Test Scenario |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~ To create the test scenario, do configure pdfmelange with "./configure --enable-testapps" ~*/

#ifdef TEST_DRAWINGAREA

#include <iostream>
#include <gtkmm.h>

/**
 * \class TestWindow
 *
 * \brief Test scenario for melangeDrawingArea.
 *
 * Workflow:
 *  1. create a main window,
 *    a. add a vertical box, insert a toolbar and a melangeDrawingArea,
 *    b. add actions to handle signals and events,
 *    c. binds actions with toolitems.
 *  2. load and view the file gpl.pdf from the current directory.
 *
 */
class TestWindow : public Gtk::Window
{
public:

    int iPage;
    int iRotate;
    PopplerPage* pPage;
    PopplerDocument* pDoc;

	//Child widgets:
    Gtk::Box m_Box;
    melangeDrawingArea drawing_area;

    Glib::RefPtr<Gtk::UIManager> m_refUIManager;
    Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;

	Glib::RefPtr<Gtk::Action> m_refAction_Go_Back;
    Glib::RefPtr<Gtk::Action> m_refAction_Go_Forward;

    TestWindow(): m_Box(Gtk::ORIENTATION_VERTICAL)
    {
        set_title("melangeDrawingArea Test Scenario");
        set_default_size(400, 594);

        // put a MenuBar at the top of the box and other stuff below it.
        add(m_Box);

        //Create actions for menus and toolbars:
        m_refActionGroup = Gtk::ActionGroup::create();

        m_refActionGroup->add(Gtk::Action::create("Open", Gtk::Stock::OPEN, "_Open", "Open PDF file"),
                              sigc::mem_fun(*this, &TestWindow::on_open));

        m_refActionGroup->add(Gtk::Action::create("Quit", Gtk::Stock::QUIT, "_Quit", "Exit"),
                              sigc::mem_fun(*this, &TestWindow::on_quit));

        m_refAction_Go_Back = Gtk::Action::create("Go_Back", Gtk::Stock::GO_BACK, "_Go_Back", "Page back");
        m_refActionGroup->add(m_refAction_Go_Back, sigc::mem_fun(*this, &TestWindow::on_go_back));

        m_refAction_Go_Forward = Gtk::Action::create("Go_Forward", Gtk::Stock::GO_FORWARD, "_Go_Forward", "Next page");
        m_refActionGroup->add(m_refAction_Go_Forward, sigc::mem_fun(*this, &TestWindow::on_go_forward));

        m_refActionGroup->add(Gtk::Action::create("Zoom_In", Gtk::Stock::ZOOM_IN, "_Zoom_In", "Zoom in"),
                              sigc::mem_fun(*this, &TestWindow::on_zoom_in));
        m_refActionGroup->add(Gtk::Action::create("Zoom_Out", Gtk::Stock::ZOOM_OUT, "_Zoom_Out", "Zoom out"),
                              sigc::mem_fun(*this, &TestWindow::on_zoom_out));
        m_refActionGroup->add(Gtk::Action::create("Zoom_Fit", Gtk::Stock::ZOOM_FIT, "_Zoom_Fit", "Zoom fit"),
                              sigc::mem_fun(*this, &TestWindow::on_zoom_fit));

        Glib::RefPtr<Gtk::Action> refAction_Rot_CW = Gtk::Action::create("Rotate_CW", "Rotate CW", "Rotate CW");
        refAction_Rot_CW->set_icon_name("object-rotate-right");
        m_refActionGroup->add(refAction_Rot_CW, sigc::mem_fun(*this, &TestWindow::on_rotate_cw));

        Glib::RefPtr<Gtk::Action> refAction_Rot_CCW = Gtk::Action::create("Rotate_CCW", "Rotate CCW", "Rotate CCW");
        refAction_Rot_CCW->set_icon_name("object-rotate-left");
        m_refActionGroup->add(refAction_Rot_CCW, sigc::mem_fun(*this, &TestWindow::on_rotate_ccw));

        m_refUIManager = Gtk::UIManager::create();
        m_refUIManager->insert_action_group(m_refActionGroup);
        add_accel_group(m_refUIManager->get_accel_group());

        //Layout the actions in the toolbar:
        Glib::ustring ui_info = "<ui>"
                                "  <toolbar  name='ToolBar'>"
                                "    <toolitem action='Open'/>"
                                "    <toolitem action='Quit'/>"
                                "    <toolitem action='Go_Back'/>"
                                "    <toolitem action='Go_Forward'/>"
                                "    <toolitem action='Zoom_In'/>"
                                "    <toolitem action='Zoom_Out'/>"
                                "    <toolitem action='Zoom_Fit'/>"
                                "    <toolitem action='Rotate_CW'/>"
                                "    <toolitem action='Rotate_CCW'/>"
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

        m_Box.pack_end(drawing_area);
        //drawing_area.set_size_request(420, 594);

        show_all_children();
        iPage = 0;
        iRotate = 0;
    };
	
    virtual ~TestWindow() {};

    void init(PopplerPage* pPage, PopplerDocument* pDoc)
    {
        this->pPage = pPage;
        this->pDoc = pDoc;
        drawing_area.set_page (pDoc, pPage, 0);
        check_sensitive();
    };

    //Signal handlers:
    void on_open()
    {
        Gtk::FileChooserDialog dialog(*this, "Please choose a file", Gtk::FILE_CHOOSER_ACTION_OPEN);
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
		        std::string filename;
		        Glib::ustring uri;
		        GError* error = NULL;

		        filename = dialog.get_filename();
		        uri = dialog.get_uri();
		        PopplerDocument* pDoc = poppler_document_new_from_file(uri.c_str(), NULL, &error);

		        if (!pDoc) {
		            printf("%s\n", error->message);
		            return;
		        }
		        int iPage = 0;
		        PopplerPage* pPage = poppler_document_get_page(pDoc, iPage);
		        init(pPage, pDoc);
		        break;
		    }
		    case(Gtk::RESPONSE_CANCEL):
		    {
		        return;
		        break;
		    }
		    default:
		    {
		        return;
		        break;
		    }
        }
    };
    void on_quit() {
        hide();
    };
    void on_go_back()
    {
        iPage--;
        if (iPage >= 0)
        {
            pPage = poppler_document_get_page(pDoc, iPage);
            if (pPage) drawing_area.set_page (pDoc, pPage, 0);
            check_sensitive();
        }
        else
            iPage++;
    };
    void on_go_forward()
    {
        iPage++;
        if (iPage < poppler_document_get_n_pages(pDoc))
        {
            pPage = poppler_document_get_page(pDoc, iPage);
            if (pPage) drawing_area.set_page (pDoc, pPage, 0);
            check_sensitive();
        }
        else
            iPage--;
    };
    void on_zoom_in()  {
        drawing_area.draw_zoom_in();
    };
    void on_zoom_out() {
        drawing_area.draw_zoom_out();
    };
    void on_zoom_fit() {
        drawing_area.draw_fitted_to_window ();
    };
    void on_rotate_cw ()  {
        if (pPage) {
            iRotate = (iRotate +  90) % 360;
            drawing_area.set_page (pDoc, pPage, iRotate);
        };
    };
    void on_rotate_ccw () {
        if (pPage) {
            iRotate = (iRotate + 270) % 360;
            drawing_area.set_page (pDoc, pPage, iRotate);
        };
    };
    void check_sensitive()
    {
        if (iPage == poppler_document_get_n_pages(pDoc) -1)
            m_refAction_Go_Forward->set_sensitive(false);
        else
            m_refAction_Go_Forward->set_sensitive(true);
        if (iPage == 0)
            m_refAction_Go_Back->set_sensitive(false);
        else
            m_refAction_Go_Back->set_sensitive(true);
    }
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

    // open a pdf file
    Glib::ustring filename = Glib::build_filename ("file:/", Glib::get_current_dir(), "gpl.pdf");
    PopplerDocument* pDoc = poppler_document_new_from_file(filename.c_str(), NULL, NULL);
    PopplerPage* pPage = NULL;

    if (pDoc) {
        int iPage = 0;
        pPage = poppler_document_get_page(pDoc, iPage);
        window.init(pPage, pDoc);
    }

    //Shows the window and returns when it is closed.
    app->run(window);

    if (pPage) g_object_unref(pPage);
    if (pDoc) g_object_unref(pDoc);

    return 0;
}

#endif