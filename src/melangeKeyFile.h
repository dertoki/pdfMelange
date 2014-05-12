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

#ifndef _MELANGEKEYFILE_H_
#define _MELANGEKEYFILE_H_

#include <glibmm/keyfile.h>

/**
 * \class melangeKeyFile
 *
 * \brief Class that handles configuration files of pdfMelange.
 *
 * Parse, edit or create files containing groups of key-value pairs.
 *
 */
class melangeKeyFile
{
public:
    melangeKeyFile();
    ~melangeKeyFile();
    melangeKeyFile(const melangeKeyFile&);
    melangeKeyFile& operator = (const melangeKeyFile&);

    void			write();
    gchar*			getFileName();

    void			loadFromFile();
    void			setIconSize(int value);
    int				getIconSize();
    void			setLogging(bool value);
    const bool		getLogging();
    void			setPageMode(Glib::ustring value);
    const char*	getPageMode();
    void			setPageLayout(Glib::ustring value);
    const char*	getPageLayout();

    static const char* m_pageLayouts[];	// { "NoLayout", "UseOutlines", "UseThumbs", "FullScreen", "UseOC", "UseAttachments"}
    static const char* m_pageModes[];		// { "NoMode", "SinglePage", "OneColumn", "TwoColumnLeft", "TwoColumnRight", "TwoPageLeft", "TwoPageRight"}
    static int		sizeofLayouts();
    static int		sizeofModes();
    static bool	isValidString(Glib::ustring value, const char* list[], int array_size);

private:
    Glib::KeyFile   m_settings; // https://developer.gnome.org/glibmm/stable/classGlib_1_1KeyFile.html
    gchar*			m_fileName;

    bool			m_logging;				// Wether use logging or not.
    char*			m_pageMode;				// PageMode mode: see Page 140 @ pdf_reference_1-7.
    char*			m_pageLayout;			// PageLayout mode: see Page 140 @ pdf_reference_1-7.
    int				m_iconSize;				// Size of icons.

    char*			getValidString(const char* key, const char* list[], int array_size, const char* defaultValue);
};

#endif // _MELANGEKEYFILE_H_
