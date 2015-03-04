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

#ifndef MELANGEPYPDFWRITER_H
#define MELANGEPYPDFWRITER_H

#include "melangePdfWriter.h"

#include "Python.h"

#include <iostream>
#include <string>
#include <sstream>
#include <list>

/**
 * \class melangePyPdfWriter
 *
 * \brief A class to merge "pdf" files.
 *
 * This class encapsulates "pyPdf" by embedding the python interpreter.
 * This version uses python version 2.7, see "http://docs.python.org/2.7/c-api".
 *
 * Dependencies:
 *
 *       * install python2.7, $ sudo apt-get install python2.7-dev
 *       * install pyPdf,     $ sudo apt-get install python-pypdf
 *       * add to link flags: -lpython27
 *
 * Example:
 *
 *       melangePyPdfWriter pdfWriter;
 *       pdfWriter.push_back( (Page){"gpl.pdf", "", 1, 0} );
 *       pdfWriter.push_back( (Page){"gpl.pdf", "", 2, 90} );
 *       pdfWriter.push_back( (Page){"gpl.pdf", "", 3, 180} );
 *       pdfWriter.writePdf("test.pdf");
 *
 */

class melangePyPdfWriter: public melangePdfWriter
{
public:
    melangePyPdfWriter();
    virtual ~melangePyPdfWriter();
    void writePdf(const char* outFileName);

    void setPageModeAndLayout(PageMode mode, PageLayout layout);
    void setPageModeAndLayout(const char* PageMode, const char* PageLayout);
 
protected:
    void onErrorThrow(const char* message);
 
private:
    PyObject* m_main; ///< the python main module
    PyObject* m_dict; ///< the python main dictionary
    PyObject* getVariable(const char* variableName); ///< Extract a variable from the interpreter.
};

#endif // MELANGEPYPDFWRITER_H
