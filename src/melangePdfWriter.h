/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * This file is part of pdfMelange.
 * Copyright (C) Tobias Tandetzki 2015 <tobzki@googlemail.com>
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

#ifndef MELANGEPDFWRITER_H
#define MELANGEPDFWRITER_H

#include <glibmm.h>

#include <list>
#include <string>
#include <iostream>

/**
 * \struct page
 *
 * \brief An item for a pages sequential list.
 *
 */
struct pageItem
{
    std::string fileName;
    std::string passWordDecrypt;
    int pageNumber;
    int pageRotate;
    int targetNumber;
    unsigned int offset;
    void *doc;
    void *page;
};

/**
 * \class melangePdfWriter base class.
 *
 * \brief A class to merge "pdf" files.
 *
 */
class melangePdfWriter: public std::list<pageItem>
{
    public:
        melangePdfWriter();
        virtual ~melangePdfWriter();

        enum PageMode	{ NoMode, UseNone, UseOutlines, UseThumbs, FullScreen, UseOC, UseAttachments};
        enum PageLayout	{ NoLayout, SinglePage, OneColumn, TwoColumnLeft, TwoColumnRight, TwoPageLeft, TwoPageRight};

        virtual void writePdf(const char* outFileName) = 0;
        virtual void setPageModeAndLayout(PageMode mode, PageLayout layout) = 0;
        virtual void setPageModeAndLayout(const char* PageMode, const char* PageLayout) = 0;

    protected:
        Glib::ustring m_strPageModeAndLayout;
        virtual void onErrorThrow(const char* message) = 0; ///< handles errors.

    private:
};

#endif // MELANGEPDFWRITER_H
