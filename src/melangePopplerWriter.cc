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

#include "melangePopplerWriter.h"
#include <glib.h>

#include <goo/GooString.h>
#include <goo/GooMutex.h>
#include <PDFDoc.h>
#include <GlobalParams.h>
#include "XRef.h"
#include "ErrorCodes.h"
#include "SecurityHandler.h"
#include <iostream>
#include <list>
#include <vector>

melangePopplerWriter::melangePopplerWriter()
{
    //ctor
}

melangePopplerWriter::~melangePopplerWriter()
{
    //dtor
}

/** \brief Merge the base list into one resulting pdf file using the poppler pdf library.
 *
 *  writePdf parses the documents list tree.
 *  + First, a structured document tree list from the given arbitrary ordered list of page items is created.
 *  + Then the tree list is executed for each document.
 *    The objects used by the pages of each document are marked and then finally written.
 *  + Next, the catalog object, the pages (tree) object and all page objects are written.
 *  + Finally, the xref table is written.
 *
 *  This method was derived from "pdfunite.cc" - poppler-0.24.5 published at http://poppler.freedesktop.org
 *
 * \param const char* outFileName, the name of the resulting merged pdf file.
 *
 */
void melangePopplerWriter::writePdf(const char* outFileName)
{
    g_message("melangePopplerWriter::writePdf");

    GooString *inFileName;
    GooString *passWord = new GooString("");
    int majorVersion = 0;
    int minorVersion = 0;

    this->setTreeList();

    // loop to set doc pointers in list.
    std::list<docItem>::iterator diter;
    for (diter = docs.begin(); diter != docs.end(); diter++)
    {
        g_message("   opened for reading: %s", diter->fileName.c_str());
        // open pdf document.
        inFileName = new GooString(diter->fileName.c_str());
        PDFDoc *doc = new PDFDoc(inFileName, passWord);
        diter->doc = doc;
        // set document pointers in each page.
        for (std::list<pageItem*>::iterator piter = diter->pages.begin(); piter != diter->pages.end(); piter++) {
            (*piter)->doc = diter->doc;
        }
        // get final pdf version.
        if (doc->isOk()) {
            if (doc->getPDFMajorVersion() > majorVersion) {
                majorVersion = doc->getPDFMajorVersion();
                minorVersion = doc->getPDFMinorVersion();
            } else if (doc->getPDFMajorVersion() == majorVersion) {
                if (doc->getPDFMinorVersion() > minorVersion) {
                    minorVersion = doc->getPDFMinorVersion();
                }
            }
        }
    }

    FILE *outFile;
    OutStream *outStr;
    int objectsCount = 0;
    Guint numOffset = 0;

    // open output file and stream.
    if (!(outFile = fopen(outFileName, "wb"))) {
        error(errIO, -1, "Could not open file '{0:s}'", outFileName);
        g_message("   Could not open file  %s", outFileName);
        return;
    }
    outStr = new FileOutStream(outFile, 0);
    g_message("   opened for writing: %s", outFileName);

    XRef *yRef = new XRef();
    XRef *countRef = new XRef();

    yRef->add(0, 65535, 0, gFalse); // initialize xref table
    PDFDoc::writeHeader(outStr, majorVersion, minorVersion); // initialize the PDF output

    // get and write pages content:
    melangePopplerWriter::iterator iter;
    for (diter = docs.begin(); diter != docs.end(); diter++)
    {
        PDFDoc *doc;
        std::list<pageItem*>::iterator piter;
        for (piter = diter->pages.begin(); piter != diter->pages.end(); piter++)
        {
            pageItem *page_item = *piter; // dereference the iterator
            doc = (PDFDoc*) diter->doc;
            int i = page_item->pageNumber;
            PDFRectangle *cropBox = NULL;

            if (doc->getCatalog()->getPage(i)->isCropped())
                cropBox = doc->getCatalog()->getPage(i)->getCropBox();
            // rewrite pageDict with MediaBox, CropBox and new page CTM
            doc->replacePageDict(i,
                                 doc->getCatalog()->getPage(i)->getRotate(),
                                 doc->getCatalog()->getPage(i)->getMediaBox(),
                                 cropBox,
                                 NULL
                                );
            Ref *refPage = doc->getCatalog()->getPageRef(i);
            // Fetch the indirect reference of Object page.
            Object *page = new Object();
            doc->getXRef()->fetch(refPage->num, refPage->gen, page);
            page_item->page = page;
            page_item->offset = numOffset;
            Dict *pageDict = page->getDict();
            doc->markPageObjects(pageDict, yRef, countRef, numOffset);
        }
        // write all objects used by pageDict to outStr
        objectsCount += doc->writePageObjects(outStr, yRef, numOffset, gTrue);
        numOffset = yRef->getNumObjects() + 1;
    }

    // write catalog object:
    int rootNum = yRef->getNumObjects() + 1;
    yRef->add(rootNum, 0, outStr->getPos(), gTrue);
    outStr->printf("%d 0 obj\n", rootNum);
    outStr->printf("<< /Type /Catalog /Pages %d 0 R", rootNum + 1);
    if ( !m_strPageModeAndLayout.empty() )
        outStr->printf("%s", m_strPageModeAndLayout.c_str());
    outStr->printf(">>\nendobj\n");
    objectsCount++;

    // write pages (tree) object.
    yRef->add(rootNum + 1, 0, outStr->getPos(), gTrue);
    outStr->printf("%d 0 obj\n", rootNum + 1);
    outStr->printf("<< /Type /Pages /Kids [");
    for (melangePopplerWriter::iterator iter = begin(); iter != end(); iter++) {
        outStr->printf(" %d 0 R", rootNum + iter->targetNumber + 1);
    }
    outStr->printf(" ] /Count %zd >>\nendobj\n", this->size());
    objectsCount++;

    // write page objects.
    for (melangePopplerWriter::iterator iter = begin(); iter != end(); iter++)
    {
        yRef->add(rootNum + iter->targetNumber + 1, 0, outStr->getPos(), gTrue);
        outStr->printf("%d 0 obj\n", rootNum + iter->targetNumber + 1);
        outStr->printf("<< ");
        Dict *pageDict = ((Object*)iter->page)->getDict();

        // Modify key "Rotate".
        const char* rotKey = "Rotate";
        Object rotObj;
        if (pageDict->hasKey(rotKey)) {
            int iRot = 0;
            pageDict->lookupInt(rotKey, NULL, &iRot);
            rotObj.initInt( (iRot + iter->pageRotate) % 360 );
            pageDict->set(rotKey, &rotObj);
        } else { // Perhaps this is unnecessary because "rotate" is always part of the page dictionary.
            rotObj.initInt(iter->pageRotate % 360);
            pageDict->add(strdup(rotKey), &rotObj);
        }

        // write page dictionary.
        for (int j = 0; j < pageDict->getLength(); j++) {
            if (j > 0) outStr->printf(" ");
            const char *key = pageDict->getKey(j);
            Object value;
            pageDict->getValNF(j, &value);
            if (strcmp(key, "Parent") == 0) {
                outStr->printf("/Parent %d 0 R", rootNum + 1);
            }
            else {
                outStr->printf("/%s ", key);
                PDFDoc::writeObject(&value, outStr, yRef, iter->offset, NULL, cryptRC4, 0, 0, 0);
            }
            value.free();
        }
        outStr->printf(" >>\nendobj\n");
        objectsCount++;
    }

    // write xref table.
    Goffset uxrefOffset = outStr->getPos();
    Ref ref;
    ref.num = rootNum;
    ref.gen = 0;
    Dict *trailerDict = PDFDoc::createTrailerDict(objectsCount, gFalse, 0, &ref, yRef,
                        outFileName, outStr->getPos());

    PDFDoc::writeXRefTableTrailer(trailerDict, yRef, gFalse /* do not write unnecessary entries */,
                                  uxrefOffset, outStr, yRef);
    g_message("   Wrote %i objects in %i pages to file %s", objectsCount, size(), outFileName);

    delete trailerDict;

    outStr->close();
    fclose(outFile);
    delete yRef;
    delete countRef;
    for (melangePopplerWriter::iterator iter = begin(); iter != end(); iter++) {
        ((Object*) iter->page)->free();
        delete (Object*) iter->page;
    }
    for (diter = docs.begin(); diter != docs.end(); diter++) {
        delete (PDFDoc*) diter->doc;
	}
}

/** \brief Create the document tree.
 *
 *  A structured document tree from the given arbitrary ordered list of page items is created.
 *  Items of the tree-list are documents and each owns a separate list of pages.
 *
 */
void melangePopplerWriter::setTreeList(void)
{
    docs.clear();

    int target_pageNumber = 0;
    melangePopplerWriter::iterator iter;
    for (iter = this->begin(); iter != this->end(); iter++)
    {
        target_pageNumber++;
        iter->targetNumber = target_pageNumber;
        // search for item of *this in mdocs
        std::list<docItem>::iterator diter;
        for (diter = docs.begin(); diter != docs.end(); diter++) {
            if (diter->fileName == iter->fileName) break;
        }
        if (diter == docs.end()) // search with no success
        {
            docItem doc;
            doc.doc = iter->doc;
            doc.fileName = iter->fileName;

            doc.pages.push_back(&*iter);

            docs.push_back(doc);
        }
        else // search with success
        {
            diter->pages.push_back(&*iter);
        }
    }
}

/** \brief print the document pages tree to std::out.
 *
 */
void melangePopplerWriter::printInfo(void)
{
    setTreeList();
    for (std::list<docItem>::iterator diter = docs.begin(); diter != docs.end(); diter++) {

        std::cout << "file name: " << diter->fileName << std::endl;
        std::cout << "  page numbers (source/target)";

        for (std::list<pageItem*>::iterator piter = diter->pages.begin(); piter != diter->pages.end(); piter++) {
            std::cout << " (" << (*piter)->pageNumber << "/" << (*piter)->targetNumber << ")";
        }
        std::cout << std::endl;
    }
}


/**
 * \brief Set the pdf viewer start configuration.
 *
 *    This method is used to modify the pdf catalog object.
 *
 * \param PageMode mode: see Page 140 @ http://www.adobe.com/content/dam/Adobe/en/devnet/acrobat/pdfs/pdf_reference_1-7.pdf
 * \param PageLayout layout: see Page 140 @ http://www.adobe.com/content/dam/Adobe/en/devnet/acrobat/pdfs/pdf_reference_1-7.pdf
 * \return void.
 */
void melangePopplerWriter::setPageModeAndLayout(melangePdfWriter::PageMode mode, melangePdfWriter::PageLayout layout)
{
    m_strPageModeAndLayout.clear();
    if (mode == NoMode && layout == NoLayout) {
        return;
    }

    switch (layout) {
        case NoMode:
            break;
        case SinglePage:
            m_strPageModeAndLayout += " /PageLayout /SinglePage";
            break;
        case OneColumn:
            m_strPageModeAndLayout += " /PageLayout /OneColumn";
            break;
        case TwoColumnLeft:
            m_strPageModeAndLayout += " /PageLayout /TwoColumnLeft";
            break;
        case TwoColumnRight:
            m_strPageModeAndLayout += " /PageLayout /TwoColumnRight";
            break;
        case TwoPageLeft:
            m_strPageModeAndLayout += " /PageLayout /TwoPageLeft";
            break;
        case TwoPageRight:
            m_strPageModeAndLayout += " /PageLayout /TwoPageRight";
            break;
    }

    if (mode != NoMode && layout != NoLayout) m_strPageModeAndLayout += ", ";

    switch (mode) {
        case NoLayout:
            break;
        case UseNone:
            m_strPageModeAndLayout += " /PageMode /UseNone";
            break;
        case UseOutlines:
            m_strPageModeAndLayout += " /PageMode /UseOutlines";
            break;
        case UseThumbs:
            m_strPageModeAndLayout += " /PageMode /UseThumbs";
            break;
        case FullScreen:
            m_strPageModeAndLayout += " /PageMode /FullScreen";
            break;
        case UseOC:
            m_strPageModeAndLayout += " /PageMode /UseOC";
            break;
        case UseAttachments:
            m_strPageModeAndLayout += " /PageMode /UseAttachments";
            break;
    }
}

/**
 * \brief Set the pdf viewer start configuration.
 *
 *    This method is used to modify the pdf catalog object.
 *
 * \param const char* PageMode (NULL if not set), see Page 140 @ http://www.adobe.com/content/dam/Adobe/en/devnet/acrobat/pdfs/pdf_reference_1-7.pdf
 * \param const char* PageLayout (NULL if not set), see Page 140 @ http://www.adobe.com/content/dam/Adobe/en/devnet/acrobat/pdfs/pdf_reference_1-7.pdf
 * \return void.
 */
void melangePopplerWriter::setPageModeAndLayout(const char* PageMode, const char* PageLayout) {

    m_strPageModeAndLayout.clear();
    if (PageMode == NULL && PageLayout == NULL) {
        return;
    }

    if (PageLayout)
        m_strPageModeAndLayout += " /PageLayout /" + Glib::ustring(PageLayout);

    if (PageMode)
        m_strPageModeAndLayout += " /PageMode /" + Glib::ustring(PageMode);

}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~| Test Scenario |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~ To create the test scenario, do configure pdfmelange with "./configure --enable-testapps" ~*/

#ifdef TEST_PDFPOPPLERWRITER

#include <iostream>

/**
 *  \brief Entry point of the test program.
 *
 *  Workflow:
 *  1. Create a melangePdfWriter instance.
 *  2. Add some elements to the melangePdfWriter instance, that is a std::list.
 *  3. Execute the write.
 *
 * \param argc: argument count, number of arguments on the command line.
 * \param *argv[]: argument vector, array of null-terminated strings,
 *                 representing the arguments that were entered on the command line
 *                 when the program was started.
 * \return -
 */
int main (int argc, char *argv[])
{
	melangePopplerWriter pdfwriter;

	pdfwriter.push_back( (pageItem){"gpl.pdf", "", 1, 0} );
	pdfwriter.push_back( (pageItem){"gpl.pdf", "", 2, 90} );
	pdfwriter.push_back( (pageItem){"gpl.pdf", "", 3, 180} );

	//pdfwriter.setPageModeAndLayout(melangePopplerWriter::UseThumbs, melangePopplerWriter::SinglePage);
	//pdfwriter.setPageModeAndLayout(melangePopplerWriter::UseNone, melangePopplerWriter::OneColumn);
	pdfwriter.setPageModeAndLayout("UseThumbs", "SinglePage");

	pdfwriter.writePdf("test.pdf");

    return 0;
}

#endif
