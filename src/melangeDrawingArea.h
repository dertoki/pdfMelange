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

#ifndef MELANGEDRAWINGAREA_H
#define MELANGEDRAWINGAREA_H

#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include <glib/poppler.h>

/** Notation:
  *
  * m_...    :Member data
  * m_p...   :Member pointer
  * m_ref... :Member reference-counting shared smartpointer
  */

/**
 * \class melangeDrawingArea
 *
 * \brief A Gtk widget to show pdf files.
 *
 * For an example, see <melangeDrawingArea.cc> "Test Scenario"
 *
 */
class melangeDrawingArea: public Gtk::DrawingArea
{
public:
    melangeDrawingArea();
    melangeDrawingArea(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);
    virtual ~melangeDrawingArea();
    /** render the pdf page using cairo. */
    void set_page(PopplerDocument* pDoc, PopplerPage* pPage, int angle);
    void unset_page();
    /** for testing: draw some lines and text with cairo */
    void _test_draw(const Cairo::RefPtr<Cairo::Context>& m_refCr);
    void draw_page_frame(const Cairo::RefPtr<Cairo::Context>& m_refCr);
    void draw_fitted_to_window();
    void draw_zoom_in();
    void draw_zoom_out();

protected:
    /** all constructors have to call init() to share the same content. */
    void init();
    /** calculate a transformation matrix to fit the pdf page insize the window. */
    Cairo::Matrix calculate_init_matrix();
    /** calculate a transformation matrix to scale at a particular point. */
    Cairo::Matrix calculate_zoom_matrix(double x, double y, double zoom);

#if   GDKMM_MAJOR_VERSION == 2
    /** Override default signal handler. */
    virtual bool on_expose_event(GdkEventExpose* event);
    Glib::RefPtr<Gdk::Window> m_refWindow;
    Cairo::RefPtr<Cairo::Context> m_refCr;
#elif GDKMM_MAJOR_VERSION == 3
    /** Override the default handler for the signal signal_draw(). */
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& m_refCr);
#endif

private:
    PopplerDocument* m_pDoc;
    PopplerPage* m_pPage;
    int m_width;
    int m_height;
    int m_pdfRotation;

    double m_pdfScale;
    double m_zoom;
	static constexpr double m_zoom_plus = 1.2f;
	static constexpr double m_zoom_minus = 0.8f;
    int m_x, m_y;
    int m_x0, m_y0;
    int m_dx, m_dy;
    Cairo::Matrix m_mtx;
    Cairo::Matrix m_mtx_zero;
    bool m_button1;

    bool on_event_scroll(GdkEventScroll *e);
    void on_my_size_allocate(Gtk::Allocation& allocation);
    virtual bool on_button_press_event(GdkEventButton* event);
    virtual bool on_button_release_event(GdkEventButton* event);
    virtual bool on_motion_notify_event(GdkEventMotion* event);
};

#endif // MELANGEDRAWINGAREA_H
