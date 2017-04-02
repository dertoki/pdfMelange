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

#include <Catalog.h>
#include <Dict.h>
#include <Error.h>
#include <glib/gmessages.h>
#include <glibmm/ustring.h>
#include <goo/gmem.h>
#include <goo/gtypes.h>
#include <goo/GooString.h>
#include <goo/GooTimer.h>
#include <Object.h>
#include <Page.h>
#include <Stream.h>
#include <XRef.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <list>
#include <string>

#include "../config.h"
#include "copyfile.h"

melangePopplerWriter::melangePopplerWriter()
{
    //ctor
}

melangePopplerWriter::~melangePopplerWriter()
{
    //dtor
}

void melangePopplerWriter::doMergeNameTree(PDFDoc *doc, XRef *srcXRef, XRef *countRef, int oldRefNum, int newRefNum, Dict *srcNameTree, Dict *mergeNameTree, int numOffset) {

    Object mergeNameArray;
    Object srcNameArray;
    mergeNameTree->lookup("Names", &mergeNameArray);
    srcNameTree->lookup("Names", &srcNameArray);
    if (mergeNameArray.isArray() && srcNameArray.isArray()) {
        Object *newNameArray = new Object();
        newNameArray->initArray(srcXRef);
        int j = 0;
        for (int i = 0; i < srcNameArray.arrayGetLength() - 1; i += 2) {
            Object key;
            Object value;
            srcNameArray.arrayGetNF(i, &key);
            srcNameArray.arrayGetNF(i + 1, &value);
            if (key.isString() && value.isRef()) {
                while (j < mergeNameArray.arrayGetLength() - 1) {
                    Object mkey;
                    Object mvalue;
                    mergeNameArray.arrayGetNF(j, &mkey);
                    mergeNameArray.arrayGetNF(j + 1, &mvalue);
                    if (mkey.isString() && mvalue.isRef()) {
                        if (mkey.getString()->cmp(key.getString()) < 0) {
                            Object *newKey = new Object();
                            newKey->initString(new GooString(mkey.getString()->getCString()));
                            newNameArray->arrayAdd(newKey);
                            Object *newValue = new Object();
                            newValue->initRef(mvalue.getRef().num + numOffset, mvalue.getRef().gen);
                            newNameArray->arrayAdd(newValue);
                            delete newKey;
                            delete newValue;
                            j += 2;
                        } else if (mkey.getString()->cmp(key.getString()) == 0) {
                            j += 2;
                        } else {
                            mkey.free();
                            mvalue.free();
                            break;
                        }
                    } else {
                        j += 2;
                    }
                    mkey.free();
                    mvalue.free();
                }
                Object *newKey = new Object();
                newKey->initString(new GooString(key.getString()->getCString()));
                newNameArray->arrayAdd(newKey);
                Object *newValue = new Object();
                newValue->initRef(value.getRef().num, value.getRef().gen);
                newNameArray->arrayAdd(newValue);
                delete newKey;
                delete newValue;
            }
            key.free();
            value.free();
        }
        while (j < mergeNameArray.arrayGetLength() - 1) {
            Object mkey;
            Object mvalue;
            mergeNameArray.arrayGetNF(j, &mkey);
            mergeNameArray.arrayGetNF(j + 1, &mvalue);
            if (mkey.isString() && mvalue.isRef()) {
                Object *newKey = new Object();
                newKey->initString(new GooString(mkey.getString()->getCString()));
                newNameArray->arrayAdd(newKey);
                Object *newValue = new Object();
                newValue->initRef(mvalue.getRef().num + numOffset, mvalue.getRef().gen);
                newNameArray->arrayAdd(newValue);
                delete newKey;
                delete newValue;
            }
            j += 2;
            mkey.free();
            mvalue.free();
        }
        srcNameTree->set("Names", newNameArray);
        doc->markPageObjects(mergeNameTree, srcXRef, countRef, numOffset, oldRefNum, newRefNum);
        delete newNameArray;
    } else if (srcNameArray.isNull() && mergeNameArray.isArray()) {
        Object *newNameArray = new Object();
        newNameArray->initArray(srcXRef);
        for (int i = 0; i < mergeNameArray.arrayGetLength() - 1; i += 2) {
            Object key;
            Object value;
            mergeNameArray.arrayGetNF(i, &key);
            mergeNameArray.arrayGetNF(i + 1, &value);
            if (key.isString() && value.isRef()) {
                Object *newKey = new Object();
                newKey->initString(new GooString(key.getString()->getCString()));
                newNameArray->arrayAdd(newKey);
                Object *newValue = new Object();
                newValue->initRef(value.getRef().num + numOffset, value.getRef().gen);
                newNameArray->arrayAdd(newValue);
                delete newKey;
                delete newValue;
            }
            key.free();
            value.free();
        }
        srcNameTree->add(copyString("Names"), newNameArray);
        doc->markPageObjects(mergeNameTree, srcXRef, countRef, numOffset, oldRefNum, newRefNum);
    }
    mergeNameArray.free();
    srcNameArray.free();
}

void melangePopplerWriter::doMergeNameDict(PDFDoc *doc, XRef *srcXRef, XRef *countRef, int oldRefNum, int newRefNum, Dict *srcNameDict, Dict *mergeNameDict, int numOffset) {

    for (int i = 0; i < mergeNameDict->getLength(); i++) {
        const char *key = mergeNameDict->getKey(i);
        Object mergeNameTree;
        Object srcNameTree;
        mergeNameDict->lookup(key, &mergeNameTree);
        srcNameDict->lookup(key, &srcNameTree);
        if (srcNameTree.isDict() && mergeNameTree.isDict()) {
            doMergeNameTree(doc, srcXRef, countRef, oldRefNum, newRefNum, srcNameTree.getDict(),
                    mergeNameTree.getDict(), numOffset);
        } else if (srcNameTree.isNull() && mergeNameTree.isDict()) {
            Object *newNameTree = new Object();
            newNameTree->initDict(srcXRef);
            doMergeNameTree(doc, srcXRef, countRef, oldRefNum, newRefNum, newNameTree->getDict(),
                    mergeNameTree.getDict(), numOffset);
            srcNameDict->add(copyString(key), newNameTree);
        }
        srcNameTree.free();
        mergeNameTree.free();
    }
}

void melangePopplerWriter::doMergeFormDict(Dict *srcFormDict, Dict *mergeFormDict, int numOffset) {

    Object srcFields, mergeFields;

    srcFormDict->lookup("Fields", &srcFields);
    mergeFormDict->lookup("Fields", &mergeFields);
    if (srcFields.isArray() && mergeFields.isArray()) {
        for (int i = 0; i < mergeFields.arrayGetLength(); i++) {
            Object value;
            Object *newValue = new Object();
            mergeFields.arrayGetNF(i, &value);
            newValue->initRef(value.getRef().num + numOffset, value.getRef().gen);
            srcFields.arrayAdd(newValue);
            value.free();
            delete newValue;
        }
    }
    srcFields.free();
    mergeFields.free();
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
 *  This method was derived from "pdfunite.cc" - poppler-0.52.0 published at http://poppler.freedesktop.org
 *
 * \param const char* outFileName, the name of the resulting merged pdf file.
 *
 */
void melangePopplerWriter::writePdf(const char* outFileName) {
    g_message("melangePopplerWriter::writePdf");
    GooTimer timer;
    timer.start();

    GooString *inFileName;
    GooString *passWord = new GooString("");
    int majorVersion = 0;
    int minorVersion = 0;

    setTreeList();
    //printInfo();

    // loop to set doc pointers in list.
    std::list<docItem>::iterator diter;
    for (diter = docs.begin(); diter != docs.end(); diter++) {
        g_message("   opened for reading: %s", diter->fileName.c_str());
        // open pdf document.
        inFileName = new GooString(diter->fileName.c_str());
        PDFDoc *doc = new PDFDoc(inFileName, passWord);
        if (!doc) {
            GooString errorMessage;
            errorMessage.appendf("Error on opening file {0:t}", inFileName);
            onErrorThrow(errorMessage.getCString());
        }
        diter->doc = doc;
        // set document pointers in each page.
        std::list<pageItem *>::iterator piter;
        for (piter = diter->pages.begin(); piter != diter->pages.end(); piter++)
            (*piter)->doc = diter->doc;
        // get final pdf version.
        if (doc->isOk()) {
            if (doc->getPDFMajorVersion() > majorVersion) {
                majorVersion = doc->getPDFMajorVersion();
                minorVersion = doc->getPDFMinorVersion();
            } else if (doc->getPDFMajorVersion() == majorVersion) {
                if (doc->getPDFMinorVersion() > minorVersion)
                    minorVersion = doc->getPDFMinorVersion();
            }
        }
    }

    int objectsCount = 0;
    Guint numOffset = 0;

    // open output file.
    FILE* tempFile = tmpfile();
    if (!tempFile) {
        error(errIO, -1, "Could not open temfile");
        onErrorThrow("Could not open tempfile");
        return;
    }
    OutStream* outStr = new FileOutStream(tempFile, 0);
    g_message("   opened tempfile for writing");


    // initialize the new PDF file.
    XRef *yRef = new XRef();
    XRef *countRef = new XRef();
    yRef->add(0, 65535, 0, gFalse);
    PDFDoc::writeHeader(outStr, majorVersion, minorVersion);

    // handle OutputIntents, AcroForm, OCProperties & Names

    Object intents;
    Object afObj;
    Object ocObj;
    Object names;
    //int i, j;
    diter = docs.begin();
    if (docs.size() >= 1) {
        Object catObj;
        PDFDoc *pdfdoc = diter->doc;
        pdfdoc->getXRef()->getCatalog(&catObj);
        Dict *catDict = catObj.getDict();
        catDict->lookup("OutputIntents", &intents);
        catDict->lookupNF("AcroForm", &afObj);
        Ref *refPage = pdfdoc->getCatalog()->getPageRef(1);
        if (!afObj.isNull()) {
            pdfdoc->markAcroForm(&afObj, yRef, countRef, 0, refPage->num, refPage->num);
        }
        catDict->lookupNF("OCProperties", &ocObj);
        if (!ocObj.isNull() && ocObj.isDict()) {
            pdfdoc->markPageObjects(ocObj.getDict(), yRef, countRef, 0, refPage->num, refPage->num);
        }
        catDict->lookup("Names", &names);
        if (!names.isNull() && names.isDict()) {
            pdfdoc->markPageObjects(names.getDict(), yRef, countRef, 0, refPage->num, refPage->num);
        }
        if (intents.isArray() && intents.arrayGetLength() > 0) {
            for (++diter; diter != docs.end(); ++diter) {
                pdfdoc = diter->doc;
                Object pagecatObj, pageintents;
                pdfdoc->getXRef()->getCatalog(&pagecatObj);
                Dict *pagecatDict = pagecatObj.getDict();
                pagecatDict->lookup("OutputIntents", &pageintents);
                if (pageintents.isArray() && pageintents.arrayGetLength() > 0) {
                    for (int j = intents.arrayGetLength() - 1; j >= 0; j--) {
                        Object intent;
                        intents.arrayGet(j, &intent, 0);
                        if (intent.isDict()) {
                            Object idf;
                            intent.dictLookup("OutputConditionIdentifier", &idf);
                            if (idf.isString()) {
                                GooString *gidf = idf.getString();
                                GBool removeIntent = gTrue;
                                for (int k = 0; k < pageintents.arrayGetLength(); k++) {
                                    Object pgintent;
                                    pageintents.arrayGet(k, &pgintent, 0);
                                    if (pgintent.isDict()) {
                                        Object pgidf;
                                        pgintent.dictLookup("OutputConditionIdentifier", &pgidf);
                                        if (pgidf.isString()) {
                                            GooString *gpgidf = pgidf.getString();
                                            if (gpgidf->cmp(gidf) == 0) {
                                                pgidf.free();
                                                removeIntent = gFalse;
                                                break;
                                            }
                                        }
                                        pgidf.free();
                                    }
                                }
                                if (removeIntent) {
                                    intents.arrayRemove(j);
                                    error(errSyntaxWarning, -1, "Output intent {0:s} missing in pdf {1:s}, removed",
                                            gidf->getCString(), pdfdoc->getFileName()->getCString());
                                }
                            } else {
                                intents.arrayRemove(j);
                                error(errSyntaxWarning, -1,
                                        "Invalid output intent dict, missing required OutputConditionIdentifier");
                            }
                            idf.free();
                        } else {
                            intents.arrayRemove(j);
                        }
                        intent.free();
                    }
                } else {
                    error(errSyntaxWarning, -1, "Output intents differs, remove them all");
                    intents.free();
                    break;
                }
                pagecatObj.free();
                pageintents.free();
            }
        }
        if (intents.isArray() && intents.arrayGetLength() > 0) {
            for (int j = intents.arrayGetLength() - 1; j >= 0; j--) {
                Object intent;
                intents.arrayGet(j, &intent, 0);
                if (intent.isDict()) {
                    pdfdoc->markPageObjects(intent.getDict(), yRef, countRef, numOffset, 0, 0);
                } else {
                    intents.arrayRemove(j);
                }
                intent.free();
            }
        }
        catObj.free();
    }

    // get and write pages content.
    for (diter = docs.begin(); diter != docs.end(); diter++) {
        PDFDoc *doc = diter->doc;
        std::list<pageItem*>::iterator piter;
        for (piter = diter->pages.begin(); piter != diter->pages.end(); piter++) {
            pageItem *page_item = *piter; // dereference the iterator
            int i = page_item->pageNumber;
            PDFRectangle *cropBox = NULL;

            if (doc->getCatalog()->getPage(i)->isCropped())
                cropBox = doc->getCatalog()->getPage(i)->getCropBox();
            // rewrite pageDict with MediaBox, CropBox and new page CTM
            doc->replacePageDict(i, doc->getCatalog()->getPage(i)->getRotate(), doc->getCatalog()->getPage(i)->getMediaBox(), cropBox);
            Ref *refPage = doc->getCatalog()->getPageRef(i);
            // Fetch the indirect reference of Object page.
            Object *page = new Object();
            doc->getXRef()->fetch(refPage->num, refPage->gen, page);
            page_item->page = page;
            page_item->offset = numOffset;
            Dict *pageDict = page->getDict();
            doc->markPageObjects(pageDict, yRef, countRef, numOffset, refPage->num, refPage->num);
        }
        // write all objects used by pageDict to outStr
        objectsCount += doc->writePageObjects(outStr, yRef, numOffset, gTrue);
        numOffset = yRef->getNumObjects() + 1;
    }

    std::vector<Object> pages;
    std::vector<Guint> offsets;
    for (diter = docs.begin(); diter != docs.end(); diter++) {
        PDFDoc *doc = diter->doc;
        std::list<pageItem*>::iterator piter;
        for (piter = diter->pages.begin(); piter != diter->pages.end(); piter++) {
            pageItem *page_item = *piter; // dereference the iterator
            int j = page_item->pageNumber;

            PDFRectangle *cropBox = NULL;
            if (doc->getCatalog()->getPage(j)->isCropped())
                cropBox = doc->getCatalog()->getPage(j)->getCropBox();
            doc->replacePageDict(j, doc->getCatalog()->getPage(j)->getRotate(),
                    doc->getCatalog()->getPage(j)->getMediaBox(), cropBox);
            Ref *refPage = doc->getCatalog()->getPageRef(j);
            Object page;
            doc->getXRef()->fetch(refPage->num, refPage->gen, &page);
            Dict *pageDict = page.getDict();
            Dict *resDict = doc->getCatalog()->getPage(j)->getResourceDict();
            if (resDict) {
                Object *newResource = new Object();
                newResource->initDict(resDict);
                pageDict->set("Resources", newResource);
                delete newResource;
            }
            pages.push_back(page);
            offsets.push_back(numOffset);
            doc->markPageObjects(pageDict, yRef, countRef, numOffset, refPage->num, refPage->num);
            Object annotsObj;
            pageDict->lookupNF("Annots", &annotsObj);
            if (!annotsObj.isNull()) {
                doc->markAnnotations(&annotsObj, yRef, countRef, numOffset, refPage->num, refPage->num);
                annotsObj.free();
            }
        }
        Object pageCatObj, pageNames, pageForm;
        doc->getXRef()->getCatalog(&pageCatObj);
        Dict *pageCatDict = pageCatObj.getDict();
        pageCatDict->lookup("Names", &pageNames);
        if (!pageNames.isNull() && pageNames.isDict()) {
            if (!names.isDict()) {
                names.free();
                names.initDict(yRef);
            }
            doMergeNameDict(doc, yRef, countRef, 0, 0, names.getDict(), pageNames.getDict(), numOffset);
        }
        pageCatDict->lookup("AcroForm", &pageForm);
        if (diter != docs.begin() && !pageForm.isNull() && pageForm.isDict()) {
            if (afObj.isNull()) {
                pageCatDict->lookupNF("AcroForm", &afObj);
            } else if (afObj.isDict()) {
                doMergeFormDict(afObj.getDict(), pageForm.getDict(), numOffset);
            }
        }
        pageForm.free();
        pageNames.free();
        pageCatObj.free();
        objectsCount += doc->writePageObjects(outStr, yRef, numOffset, gTrue);
        numOffset = yRef->getNumObjects() + 1;
    }

    int rootNum = yRef->getNumObjects() + 1;
    // write catalog object:
    {
        yRef->add(rootNum, 0, outStr->getPos(), gTrue);
        outStr->printf("%d 0 obj\n", rootNum);
        outStr->printf("<< /Type /Catalog /Pages %d 0 R", rootNum + 1);
        if (!m_strPageModeAndLayout.empty())
            outStr->printf("%s", m_strPageModeAndLayout.c_str());
        // insert OutputIntents
        if (intents.isArray() && intents.arrayGetLength() > 0) {
          outStr->printf(" /OutputIntents [");
          for (int j = 0; j < intents.arrayGetLength(); j++) {
            Object intent;
            intents.arrayGet(j, &intent, 0);
            if (intent.isDict()) {
              PDFDoc::writeObject(&intent, outStr, yRef, 0, NULL, cryptRC4, 0, 0, 0);
            }
            intent.free();
          }
          outStr->printf("]");
        }
        intents.free();
        // insert AcroForm
        if (!afObj.isNull()) {
          outStr->printf(" /AcroForm ");
          PDFDoc::writeObject(&afObj, outStr, yRef, 0, NULL, cryptRC4, 0, 0, 0);
          afObj.free();
        }
        // insert OCProperties
        if (!ocObj.isNull() && ocObj.isDict()) {
          outStr->printf(" /OCProperties ");
          PDFDoc::writeObject(&ocObj, outStr, yRef, 0, NULL, cryptRC4, 0, 0, 0);
          ocObj.free();
        }
        // insert Names
        if (!names.isNull() && names.isDict()) {
          outStr->printf(" /Names ");
          PDFDoc::writeObject(&names, outStr, yRef, 0, NULL, cryptRC4, 0, 0, 0);
          names.free();
        }
        outStr->printf(">>\nendobj\n");
        objectsCount++;
    }

    // write pages (tree) object.
    {
        yRef->add(rootNum + 1, 0, outStr->getPos(), gTrue);
        outStr->printf("%d 0 obj\n", rootNum + 1);
        outStr->printf("<< /Type /Pages /Kids [");
        for (melangePopplerWriter::iterator iter = begin(); iter != end(); iter++) {
            outStr->printf(" %d 0 R", rootNum + iter->targetNumber + 1); // (targetnumber starts with 1)
        }
        outStr->printf(" ] /Count %zd >>\nendobj\n", this->size());
        objectsCount++;
    }

    // write page objects.
    for (melangePopplerWriter::iterator iter = begin(); iter != end(); iter++) {
        yRef->add(rootNum + iter->targetNumber + 1, 0, outStr->getPos(), gTrue);
        outStr->printf("%d 0 obj\n", rootNum + iter->targetNumber + 1);
        outStr->printf("<< ");
        Dict *pageDict = ((Object*) iter->page)->getDict();

        // Modify key "Rotate".
        const char* rotKey = "Rotate";
        Object rotObj;
        if (pageDict->hasKey(rotKey)) {
            int iRot = 0;
            pageDict->lookupInt(rotKey, NULL, &iRot);
            rotObj.initInt((iRot + iter->pageRotate) % 360);
            pageDict->set(rotKey, &rotObj);
        } else { // Perhaps this is unnecessary because "rotate" is always part of the page dictionary.
            rotObj.initInt(iter->pageRotate % 360);
            pageDict->add(strdup(rotKey), &rotObj);
        }

        // write page dictionary.
        for (int j = 0; j < pageDict->getLength(); j++) {
            if (j > 0)
                outStr->printf(" ");
            const char *key = pageDict->getKey(j);
            Object value;
            pageDict->getValNF(j, &value);
            if (strcmp(key, "Parent") == 0) {
                outStr->printf("/Parent %d 0 R", rootNum + 1);
            } else {
                outStr->printf("/%s ", key);
                PDFDoc::writeObject(&value, outStr, yRef, iter->offset, NULL, cryptRC4, 0, 0, 0);
            }
            value.free();
        }
        outStr->printf(" >>\nendobj\n");
        objectsCount++;
    }

    // create the trailer.
    // <pdf_reference 1.7 - 3.4.4 File Trailer>.
    {
        Goffset uxrefOffset = outStr->getPos();
        Ref ref;
        ref.num = rootNum;
        ref.gen = 0;
        Dict *trailerDict = PDFDoc::createTrailerDict(objectsCount, gFalse, 0, &ref, yRef,
                                                      outFileName, outStr->getPos());
        PDFDoc::writeXRefTableTrailer(trailerDict, yRef, gTrue, // write all entries according to ISO 32000-1, 7.5.4 Cross-Reference Table: "For a file that has never been incrementally updated, the cross-reference section shall contain only one subsection, whose object numbering begins at 0."
                                      uxrefOffset, outStr, yRef);
        delete trailerDict;
    }

    g_message("   Wrote %i objects in %i pages", objectsCount, (int ) size());

    // cleanup
    delete yRef;
    delete countRef;
    for (melangePopplerWriter::iterator iter = begin(); iter != end(); iter++) {
        ((Object*) iter->page)->free();
        delete (Object*) iter->page;
    }

    timer.stop();
    g_message("%f seconds for writing %s", timer.getElapsed(), "Temp File");
    timer.start();
#ifndef __WIN32
    // @fixme: does not work with mingw:
    //         This chauses a crash with poppler 0.24.5 and mingw (gtk+ 3.10.4).
    // @todo: check with current poppler version.
    for (diter = docs.begin(); diter != docs.end(); diter++) {
        delete (PDFDoc*) diter->doc;
    }
#endif

    outStr->close();

    timer.stop();
    g_message("%f seconds for writing %s", timer.getElapsed(), "Temp File");
    timer.start();
    // copy temp file to output
    rewind(tempFile); // positions the stream tempFile at its beginning.

    FILE* outFile = fopen(outFileName, "wb");
    if (!outFile) {
        GooString errorMessage;
        errorMessage.appendf("Error on opening file {0:s}", outFileName);
        onErrorThrow(errorMessage.getCString());
    }
    copyfile(tempFile, outFile);

    fclose(tempFile);
    fclose(outFile);

    timer.stop();
    g_message("%f seconds for writing %s", timer.getElapsed(), outFileName);
}

/** \brief Create the document tree.
 *
 *  A structured document tree from the given arbitrary ordered list of page items is created.
 *  Items of the tree-list are documents and each owns a separate list of pages.
 *
 */
void melangePopplerWriter::setTreeList(void) {
    docs.clear();

    int target_pageNumber = 0;
    melangePopplerWriter::iterator iter;
    for (iter = this->begin(); iter != this->end(); iter++) {
        target_pageNumber++;
        iter->targetNumber = target_pageNumber;
        // search for item of *this in mdocs
        std::list<docItem>::iterator diter;
        for (diter = docs.begin(); diter != docs.end(); diter++) {
            if (diter->fileName == iter->fileName)
                break;
        }
        if (diter == docs.end()) { // search with no success
            docItem doc;
            doc.doc = (PDFDoc*) iter->doc;
            doc.fileName = iter->fileName;

            doc.pages.push_back(&*iter);

            docs.push_back(doc);
        } else { // search with success
            diter->pages.push_back(&*iter);
        }
    }
}

/** \brief print the document pages tree to std::out.
 *
 */
void melangePopplerWriter::printInfo(void) {
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
void melangePopplerWriter::setPageModeAndLayout(melangePdfWriter::PageMode mode, melangePdfWriter::PageLayout layout) {
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

    if (mode != NoMode && layout != NoLayout)
        m_strPageModeAndLayout += ", ";

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
void melangePopplerWriter::setPageModeAndLayout(const char* PageMode,
        const char* PageLayout) {

    m_strPageModeAndLayout.clear();
    if (PageMode == NULL && PageLayout == NULL) {
        return;
    }

    if (PageLayout)
        m_strPageModeAndLayout += " /PageLayout /" + Glib::ustring(PageLayout);

    if (PageMode)
        m_strPageModeAndLayout += " /PageMode /" + Glib::ustring(PageMode);

}

/**
 * \brief A wrapper to throw.
 *
 *     If there are errors on poppler send a g_message and throw an exception.
 *
 * \param const char* : intro text for the error message.
 * \throw char* : error message text.
 * \return void
 */
void melangePopplerWriter::onErrorThrow(const char* message) {
    g_message("  popplerWriter Error: %s", message);
    throw strdup(message);
    return;
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
int main (int argc, char *argv[]) {
    melangePopplerWriter pdfwriter;

    pdfwriter.push_back( (pageItem) {"minimal.pdf", "", 1, 90});
    pdfwriter.push_back( (pageItem) {"AcroForm-example.pdf", "", 1, 0});
    pdfwriter.push_back( (pageItem) {"gpl.pdf", "", 1, 0});
    pdfwriter.push_back( (pageItem) {"gpl.pdf", "", 2, 90});
    pdfwriter.push_back( (pageItem) {"minimal.pdf", "", 1, 0});
    pdfwriter.push_back( (pageItem) {"gpl.pdf", "", 3, 180});

    //pdfwriter.setPageModeAndLayout(melangePopplerWriter::UseThumbs, melangePopplerWriter::SinglePage);
    //pdfwriter.setPageModeAndLayout(melangePopplerWriter::UseNone, melangePopplerWriter::OneColumn);
    pdfwriter.setPageModeAndLayout("UseThumbs", "SinglePage");

    pdfwriter.writePdf("test.pdf");

    return 0;
}

#endif
