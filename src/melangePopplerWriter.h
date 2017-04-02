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

#ifndef MELANGEPOPPLERWRITER_H
#define MELANGEPOPPLERWRITER_H

#include "melangePdfWriter.h"
#include <PDFDoc.h>

/**
 * \struct mdoc
 *
 * \brief An item for a document tree list.
 *
 */
struct docItem {
    PDFDoc* doc;
	std::string fileName;
	std::list<pageItem*> pages;
};

/**
 * \class melangePopplerWriter
 *
 * \brief A class to merge "pdf" files.
 *
 * A class that uses the library poppler to write pdf documents.
 *
 * Example:
 *
 *       melangePopplerWriter pdfwriter;
 *       pdfwriter.push_back( (pageItem){"gpl.pdf", "", 1, 0} );
 *       pdfwriter.push_back( (pageItem){"gpl.pdf", "", 2, 90} );
 *       pdfwriter.push_back( (pageItem){"gpl.pdf", "", 3, 180} );
 *       pdfwriter.writePdf("test.pdf");
 *
 */

class melangePopplerWriter: public melangePdfWriter {
public:
	melangePopplerWriter();
	virtual ~melangePopplerWriter();

	void writePdf(const char* outFileName);
	void setPageModeAndLayout(PageMode mode, PageLayout layout);
	void setPageModeAndLayout(const char* PageMode, const char* PageLayout);
	void printInfo(void);

protected:
	std::list<docItem> docs;   // source documents tree

	void onErrorThrow(const char* message);
	void setTreeList(void);

private:
	void doMergeNameTree(PDFDoc *doc, XRef *srcXRef, XRef *countRef, int oldRefNum, int newRefNum, Dict *srcNameTree, Dict *mergeNameTree, int numOffset);
	void doMergeNameDict(PDFDoc *doc, XRef *srcXRef, XRef *countRef, int oldRefNum, int newRefNum, Dict *srcNameDict, Dict *mergeNameDict, int numOffset);
	void doMergeFormDict(Dict *srcFormDict, Dict *mergeFormDict, int numOffset);
};

#endif // MELANGEPOPPLERWRITER_H
