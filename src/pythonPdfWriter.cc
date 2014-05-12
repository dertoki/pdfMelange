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

#include "pythonPdfWriter.h"
#include "glib.h"

/**
 * \brief Default constructor.
 */
pythonPdfWriter::pythonPdfWriter()
{
    g_message("pythonPdfWriter constructor");
    /* Initialize the Python interpreter. */
    Py_Initialize();

    m_main = PyImport_AddModule("__main__");
    m_dict = PyModule_GetDict(m_main);

    m_strPageModeAndLayout.clear();
}

/**
 * \brief Default destructor.
 */
pythonPdfWriter::~pythonPdfWriter()
{
    g_message("pythonPdfWriter destructor");
    /* Undo all initializations made by Py_Initialize() */
    Py_Finalize();
}

/**
 * \brief Merge the list (this) and write to a pdf file.
 *
 * \param const char* outFileName: Name of the output file. Must have extension ".pdf".
 * \return void
 */
void pythonPdfWriter::writePdf(const char* outFileName)
{
    g_message("pythonPdfWriter::writePdf");

    pythonPdfWriter::iterator iter;

    /** include the python libraries (in module __main__) */
    /** create a PdfFileWriter. */
    PyObject* result = PyRun_String(
                           "from pyPdf import PdfFileWriter, PdfFileReader \n"
                           "from pyPdf.generic import NameObject           \n"
                           "from tempfile import NamedTemporaryFile        \n"
                           "from shutil import move                        \n"
                           "from os import remove                          \n"
                           "pdfOutput = PdfFileWriter()                    \n"
                           "inputFileList = {}"
                           , Py_file_input, m_dict, m_dict);
    Py_XDECREF(result);
    onErrorThrow("python ERROR while init:");

    /** set pdf viewer start configuration. */
    if ( !m_strPageModeAndLayout.empty() )
    {
        result = PyRun_String( m_strPageModeAndLayout.c_str(), Py_file_input, m_dict, m_dict);
        Py_XDECREF(result);
        onErrorThrow("python ERROR setting reader start configuration:");
    }

    //throw strdup("zefix"); // create an error.

    /** loop the pages list. */
    for (iter = this->begin(); iter != this->end(); iter++)
    {
        PyObject* my_fileName = PyUnicode_FromString(iter->fileName.c_str());
        PyObject_SetAttrString(m_main, "fileName", my_fileName);

        PyObject* my_passWordDecrypt = PyString_FromString(iter->passWordDecrypt.c_str());
        //PyObject* my_passWordDecrypt = PyUnicode_FromString(iter->passWordDecrypt.c_str());
        PyObject_SetAttrString(m_main, "passWordDecrypt", my_passWordDecrypt);

        PyObject* my_pageNumber = PyInt_FromLong(iter->pageNumber);
        PyObject_SetAttrString(m_main, "pageNumber", my_pageNumber);

        PyObject* my_pageRotation  = PyInt_FromLong(iter->pageRotate);
        PyObject_SetAttrString(m_main, "pageRotation", my_pageRotation);

        Py_XDECREF(my_fileName);
        Py_XDECREF(my_passWordDecrypt);
        Py_XDECREF(my_pageNumber);
        Py_XDECREF(my_pageRotation);

        g_message("   page %2i with rotation %3i of file %s", iter->pageNumber, iter->pageRotate, iter->fileName.c_str());

        result = PyRun_String(
                     /** do open input file only when not opened before. */
                     "if fileName not in inputFileList:               \n"
                     "   inputFile =  file( fileName, \"rb\")         \n"
                     "   inputFileList[ fileName ] = inputFile        \n"
                     "else:                                           \n"
                     "   inputFile = inputFileList[ fileName ]        \n"
                     /** do the pdf merge. */
                     "pdfInput = PdfFileReader( inputFile )           \n"
                     "if passWordDecrypt:				                \n"
                     "   print pdfInput.isEncrypted                   \n"
                     "   pdfInput.decrypt( passWordDecrypt )          \n"
                     "   print pdfInput.isEncrypted                   \n"
                     "pdfPage = pdfInput.getPage( pageNumber - 1 )    \n"
                     "pdfPage.rotateCounterClockwise( -pageRotation ) \n"
                     "pdfOutput.addPage( pdfPage )"
                     , Py_file_input, m_dict, m_dict);
        Py_XDECREF(result);

        onErrorThrow("python ERROR while merge:");
    }

    PyObject* my_outFileName = PyUnicode_FromString(outFileName);
    PyObject_SetAttrString(m_main, "outFileName", my_outFileName);

    /** write the pdf to a temp file.*/
    result = PyRun_String(
                 "tmpFile = NamedTemporaryFile( delete=False )      \n"
                 "tmpFileName = tmpFile.name                        \n"
//                 "print \"temp file name: \",  tmpFile.name         \n"
                 "pdfOutput.write( tmpFile )                        \n"
                 "tmpFile.close()                                   \n"
                 , Py_file_input, m_dict, m_dict);
    Py_XDECREF(result);
    onErrorThrow("python ERROR while writing to temp file:");

    /** close all input files (that are still open for reading). */
    result = PyRun_String(
//                 "print \"   input files: \",  len( inputFileList ) \n"
                 "for inputFileName  in inputFileList:              \n"
                 "    inputFileList[ inputFileName ].close()        \n"
//                 "    print \"closed input file: \",  inputFileName \n"
                 , Py_file_input, m_dict, m_dict);
    Py_XDECREF(result);
    onErrorThrow("python ERROR while closing input files:");

    /** copy temp file to specified output file. */
    result = PyRun_String(
//                 "print \" out file name: \",  outFileName          \n"
                 "move( tmpFileName, outFileName )"
                 , Py_file_input, m_dict, m_dict);
    Py_XDECREF(result);
    onErrorThrow("python ERROR while copy temp file to output file:");


    PyObject* my_tmpFileName = PyString_FromString("");
    my_tmpFileName = getVariable("tmpFileName");
    char* tmpFileName = PyString_AS_STRING(my_tmpFileName);
    g_message("   python generated temp file: %s", tmpFileName);

    return;
}

/**
 * \brief Set the pdf viewer start configuration.
 *
 *    This method creates a string (m_strPageModeAndLayout) for pyPdf (1.3) to edit the pdf header.
 *    m_strPageModeAndLayout is applied to PyRun_String, see pythonPdfWriter::writePdf.
 *
 *    Result example:
 *      "root = pdfOutput.getObject(pdfOutput._root)\n"
 *      "root.update({NameObject('/PageLayout'): NameObject('/SinglePage'), NameObject('/PageMode'): NameObject('/UseThumbs')})"
 *
 * \param PageMode mode: see Page 140 @ http://www.adobe.com/content/dam/Adobe/en/devnet/acrobat/pdfs/pdf_reference_1-7.pdf
 * \param PageLayout layout: see Page 140 @ http://www.adobe.com/content/dam/Adobe/en/devnet/acrobat/pdfs/pdf_reference_1-7.pdf
 * \return void.
 */
void pythonPdfWriter::setPageModeAndLayout(PageMode mode, PageLayout layout)
{
    if (mode == NoMode && layout == NoLayout) {
        m_strPageModeAndLayout.clear();
        return;
    }

    m_strPageModeAndLayout =  "root = pdfOutput.getObject(pdfOutput._root)\n";
    m_strPageModeAndLayout += "root.update({";

    switch (layout) {
    case NoMode:
        break;
    case SinglePage:
        m_strPageModeAndLayout += "NameObject('/PageLayout'): NameObject('/SinglePage')";
        break;
    case OneColumn:
        m_strPageModeAndLayout += "NameObject('/PageLayout'): NameObject('/OneColumn')";
        break;
    case TwoColumnLeft:
        m_strPageModeAndLayout += "NameObject('/PageLayout'): NameObject('/TwoColumnLeft')";
        break;
    case TwoColumnRight:
        m_strPageModeAndLayout += "NameObject('/PageLayout'): NameObject('/TwoColumnRight')";
        break;
    case TwoPageLeft:
        m_strPageModeAndLayout += "NameObject('/PageLayout'): NameObject('/TwoPageLeft')";
        break;
    case TwoPageRight:
        m_strPageModeAndLayout += "NameObject('/PageLayout'): NameObject('/TwoPageRight')";
        break;
    }

    if (mode != NoMode && layout != NoLayout) m_strPageModeAndLayout += ", ";

    switch (mode) {
    case NoLayout:
        break;
    case UseNone:
        m_strPageModeAndLayout += "NameObject('/PageMode'): NameObject('/UseNone')";
        break;
    case UseOutlines:
        m_strPageModeAndLayout += "NameObject('/PageMode'): NameObject('/UseOutlines')";
        break;
    case UseThumbs:
        m_strPageModeAndLayout += "NameObject('/PageMode'): NameObject('/UseThumbs')";
        break;
    case FullScreen:
        m_strPageModeAndLayout += "NameObject('/PageMode'): NameObject('/FullScreen')";
        break;
    case UseOC:
        m_strPageModeAndLayout += "NameObject('/PageMode'): NameObject('/UseOC')";
        break;
    case UseAttachments:
        m_strPageModeAndLayout += "NameObject('/PageMode'): NameObject('/UseAttachments')";
        break;
    }
    m_strPageModeAndLayout += "})";
}

/**
 * \brief Set the pdf viewer start configuration.
 *
 *  This method creates a string (m_strPageModeAndLayout) for pyPdf (1.3) to edit the pdf header.
 *  m_strPageModeAndLayout is applied to PyRun_String, see pythonPdfWriter::writePdf.
 *  If both PageMode and PageLayout are NULL then m_strPageModeAndLayout is cleared (set to "").
 *
 *  Result example:
 *     "root = pdfOutput.getObject(pdfOutput._root)\n"
 *     "root.update({NameObject('/PageLayout'): NameObject('/SinglePage'), NameObject('/PageMode'): NameObject('/UseThumbs')})"
 *
 * \param const char* PageMode (NULL if not set), see Page 140 @ http://www.adobe.com/content/dam/Adobe/en/devnet/acrobat/pdfs/pdf_reference_1-7.pdf
 * \param const char* PageLayout (NULL if not set), see Page 140 @ http://www.adobe.com/content/dam/Adobe/en/devnet/acrobat/pdfs/pdf_reference_1-7.pdf
 * \return void.
 */
void pythonPdfWriter::setPageModeAndLayout(const char* PageMode, const char* PageLayout)
{
    //printf("mode %s layout %s\n", PageMode, PageLayout);

    if (PageMode == NULL && PageLayout == NULL) {
        m_strPageModeAndLayout.clear();
        return;
    }

    m_strPageModeAndLayout =  "root = pdfOutput.getObject(pdfOutput._root)\n";
    m_strPageModeAndLayout += "root.update({";

    if (PageLayout)
        m_strPageModeAndLayout += "NameObject('/PageLayout'): NameObject('/" + Glib::ustring(PageLayout) + "')";

    if (PageMode && PageLayout) m_strPageModeAndLayout += ", ";

    if (PageMode)
        m_strPageModeAndLayout += "NameObject('/PageMode'): NameObject('/" + Glib::ustring(PageMode) + "')";

    m_strPageModeAndLayout += "})";
}

/**
 * \brief Extract a variable from the interpreter.
 *
 *     Derived from http://www.elmer.sourceforge.net/PyCon04/elmer_pycon04.html
 *
 * Example:
 *     PyObject* my_pageNumber = PyInt_FromLong(0);
 *     my_pageNumber = getVariable("pageNumber");
 *     long pageNumber = PyInt_AsLong(my_pageNumber);
 *     std::cout << pageNumber << std::endl;
 *
 * \param const char* valueName: the name of the variable
 * \return PyObject*: the current pointer to the variable.
 */
PyObject* pythonPdfWriter::getVariable(const char* variableName)
{
    PyObject* evalModule = PyImport_AddModule( (char*)"__main__" );
    PyObject* evalDict   = PyModule_GetDict( evalModule );
    PyObject* evalVal    = PyDict_GetItemString( evalDict, variableName );
    return evalVal;
}

/**
 * \brief Check if an error has been occurred on python.
 *
 *     If there are errors on python send a g_message and throw an exception.
 *
 * \param const char* : intro text for the error message.
 * \throw char* : error message text.
 * \return void
 */
void pythonPdfWriter::onErrorThrow(const char* message)
{
    PyObject *error = PyErr_Occurred();
    if (error != NULL) {
        PyObject *ptype, *pvalue, *ptraceback;
        PyErr_Fetch(&ptype, &pvalue, &ptraceback);
        char* py_message = g_strdup(PyString_AsString(pvalue));

        char* errorMessage = g_strdup_printf("%s %s", message, py_message);
        g_message("   %s %s", message, py_message);

        Py_XDECREF(ptype);
        Py_XDECREF(pvalue);
        Py_XDECREF(ptraceback);
        free(py_message);
        throw strdup(errorMessage);
    }

    return;
}
