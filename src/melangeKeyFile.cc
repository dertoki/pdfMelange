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

#include <string.h>
#include <fstream>
#include <glibmm.h>

#define ICON_SIZE_MIN 4
#define ICON_SIZE_MAX 501

const char* melangeKeyFile::m_pageLayouts[] =	{"SinglePage", "OneColumn", "TwoColumnLeft", "TwoColumnRight", "TwoPageLeft", "TwoPageRight"};
const char* melangeKeyFile::m_pageModes[] =	{"UseNone", "UseOutlines", "UseThumbs", "FullScreen", "UseOC", "UseAttachments"};

/**
 * \brief constructor
 */
melangeKeyFile::melangeKeyFile()
{
    // default values:
    m_logging = false;
    m_iconSize = 80;
    m_pageMode = strdup(m_pageModes[2]);
    m_pageLayout = strdup(m_pageLayouts[0]);

    // create the config directory if it does not exists.
    m_fileName = g_build_filename(g_get_user_config_dir(), "pdfmelange", NULL);
    g_mkdir_with_parents(m_fileName, 0700);
    g_free(m_fileName);

    // create the config file name.
    m_fileName = g_build_filename(g_get_user_config_dir(), "pdfmelange", "pdfmelange.conf", NULL);
}

/**
 * \brief destructor
 */
melangeKeyFile::~melangeKeyFile()
{
    g_free(m_fileName);
    free(m_pageMode);
    free(m_pageLayout);
}

/**
 * \brief copy constructor
 */
melangeKeyFile::melangeKeyFile(const melangeKeyFile& value)
{
    m_logging = value.m_logging;
    m_iconSize = value.m_iconSize;
    m_pageMode = strdup(value.m_pageMode);
    m_pageLayout = strdup(value.m_pageLayout);
    m_fileName = strdup(value.m_fileName);
}

/**
 * \brief assignment operator
 */
melangeKeyFile& melangeKeyFile::operator = (const melangeKeyFile& value)
{
    m_logging = value.m_logging;
    m_iconSize = value.m_iconSize;

    free(m_pageMode);
    free(m_pageLayout);
    free(m_fileName);
    m_pageMode = strdup(value.m_pageMode);
    m_pageLayout = strdup(value.m_pageLayout);
    m_fileName = strdup(value.m_fileName);

    return *this;
}

/**
 * \brief Read the configuration from a file.
 */
void melangeKeyFile::loadFromFile()
{
    // read the config file.
    try {
        m_settings.load_from_file(m_fileName);
    }
    catch (Glib::Error error) {
        printf("Oops! Error on opening configuration file: \"%s\"\n", error.what().c_str());
        this->write();
        printf("  > %s\n", "A new configfile was generated.");
    }

    try {
        int value = m_settings.get_integer("pdfmelange", "iconsize");
        if (value > ICON_SIZE_MIN && value < ICON_SIZE_MAX) m_iconSize = value;
    } catch (Glib::KeyFileError error) {
        printf("%s\n", error.what().c_str());
        printf("'iconsize' was set to default: %i\n", m_iconSize);
    }
    try {
        m_logging = m_settings.get_boolean("pdfmelange", "logging");
    } catch (Glib::KeyFileError error) {
        printf("%s\n", error.what().c_str());
        printf("'logging' was set to default: %s\n", m_logging ? "true" : "false");
    }
    free(m_pageLayout);
    m_pageLayout = getValidString("pageLayout", m_pageLayouts, sizeof(m_pageLayouts) / sizeof(*m_pageLayouts), m_pageLayouts[0]);
    free(m_pageMode);
    m_pageMode = getValidString("pageMode", m_pageModes, sizeof(m_pageModes) / sizeof(*m_pageModes), m_pageModes[2]);
}

/**
 * \brief Getter method for filename.
 *
 * \return a newly allocated copy of m_filename.
 */
gchar* melangeKeyFile::getFileName()
{
    return strdup(m_fileName);
}

/**
 * \brief Write the configuration to a file.
 */
void melangeKeyFile::write()
{
    m_settings.set_boolean("pdfmelange", "logging",  m_logging);
    m_settings.set_integer("pdfmelange", "iconsize", m_iconSize);
    m_settings.set_string("pdfmelange", "pageMode", m_pageMode);
    m_settings.set_string("pdfmelange", "pageLayout", m_pageLayout);

    gchar *cstr = g_date_time_format(g_date_time_new_now_local(), " pdfMelange configfile created at %F - %T");
    m_settings.set_comment(cstr);
    g_free(cstr);

    std::ofstream os(m_fileName);
    os << m_settings.to_data();
}

/**
 * \brief Calculate the size of the static (class) variable array m_pageLayouts.
 *
 * \return int size
 */
int melangeKeyFile::sizeofLayouts()
{
    return sizeof(m_pageLayouts) / sizeof(*m_pageLayouts);
}

/**
 * \brief Calculate the size of the static (class) variable array m_pageModes.
 *
 * \return int size
 */
int melangeKeyFile::sizeofModes()
{
    return sizeof(m_pageModes) / sizeof(*m_pageModes);
}

/**
 * \brief Get string value to key from configfile. Check the value for validity.
 *
 *  Helper function.
 *
 * \param char* key
 * \param char** list of valid values
 * \param int size of list of valid values ( sizeof(list) / sizeof(*list) )
 * \param defaultValue to key, is set and returned when value is invalid.
 * \return char* value: if value is valid for key, default if value is invalid for key, NULL if key was not found.
 */
char* melangeKeyFile::getValidString(const char* key, const char* list[], int list_size, const char* defaultValue)
{
    try {
        Glib::ustring value = m_settings.get_string("pdfmelange", key);

        // Check if value is an element of list.
        int i;
        for (i = 0; i < list_size; i++) {
            if (0 == value.compare(list[i])) break;
        }
        if (i == list_size) {
            Glib::ustring message = "Error in configfile: Key '" + Glib::ustring(key) + "' with wrong value '" + value + "'";
            throw Glib::KeyFileError(Glib::KeyFileError::Code::INVALID_VALUE, message.c_str());
        }
        return strdup(value.c_str());
    } catch (Glib::KeyFileError error) {
        if (error.code() == Glib::KeyFileError::Code::INVALID_VALUE) {
            printf("%s\n", error.what().c_str());
            printf("'%s' was set to default: '%s'\n", key, defaultValue);
            return strdup(defaultValue);
        } else { //if (error.code == Glib::KeyFileError::Code::KEY_NOT_FOUND) {
            //printf("%s\n", error.what().c_str());
            return NULL;
        }
    }
}

/**
 * \brief Check the value string for validity.
 *
 *  Helper function.
 *
 * \param Glib::ustring value
 * \param char** list of valid values
 * \param int size of list of valid values ( sizeof(list) / sizeof(*list) )
 * \return true: if value is valid, false if value is invalid.
 */
bool melangeKeyFile::isValidString(Glib::ustring value, const char* list[], int list_size)
{
    // Check if value is an element of list.
    int i;
    for (i = 0; i < list_size; i++) {
        if (0 == value.compare(list[i])) break;
    }
    if (i == list_size) {
        printf("Error when trying to set wrong configuration value '%s'\n", value.c_str());
        return false;
    }
    return true;
}

/**
 * \brief Get the value for key "iconsize" from the configfile.
 *
 *  Get the value, check for validity, set default on error.
 *
 * \return int iconSize.
 */
int melangeKeyFile::getIconSize()
{
    return m_iconSize;
}

/**
 * \brief Set the value for key "iconsize".
 *
 *  Check for validity.
 *
 * \param int size of icons.
 * \return void.
 */
void melangeKeyFile::setIconSize(int value)
{
    if (value > ICON_SIZE_MIN && value < ICON_SIZE_MAX)
        m_iconSize = value;
}

/**
 * \brief Get the value for key "logging" from the configfile.
 *
 *  Get the value, set default on error.
 *
 * \return true when logging was set, otherwise false.
 */
const bool melangeKeyFile::getLogging()
{
    return m_logging;
}

/**
 * \brief Set the value for key "logging".
 *
 * \param true or false.
 * \return void.
 */
void melangeKeyFile::setLogging(bool value)
{
    m_logging = value;
}

/**
 * \brief Get the value for key "pageMode" from the configfile.
 *
 *  Get the value, check for validity, set default on error.
 *
 * \return a new allocated string (default value on error) or NULL if key was not found.
 */
const char* melangeKeyFile::getPageMode()
{
    return strdup(m_pageMode);
}

/**
 * \brief Set the value for key "pageMode".
 *
 *  Check for validity, set default on error.
 *
 * \param Glib::ustring value
 * \return void
 */
void melangeKeyFile::setPageMode(Glib::ustring value)
{
    if (isValidString(value, m_pageModes, sizeof(m_pageModes) / sizeof(*m_pageModes))) {
        free(m_pageMode);
        m_pageMode = strdup(value.c_str());
    }
    else {
        printf("'pageMode' was set to default: '%s'\n", m_pageMode);
    }
}

/**
 * \brief Get the value for key "pageLayout" from the configfile.
 *
 *  Get the value, check for validity, set default on error.
 *
 * \return a new allocated string (default value on error) or NULL if key was not found.
 */
const char* melangeKeyFile::getPageLayout()
{
    return strdup(m_pageLayout);
}

/**
 * \brief Set the value for key "pageLayout".
 *
 *  Check for validity, set default on error.
 *
 * \param Glib::ustring value
 * \return void
 */
void melangeKeyFile::setPageLayout(Glib::ustring value)
{
    if (isValidString(value, m_pageLayouts, sizeof(m_pageLayouts) / sizeof(*m_pageLayouts))) {
        free(m_pageLayout);
        m_pageLayout = strdup(value.c_str());
    }
    else {
        printf("'pageLayout' was set to default: '%s'\n", m_pageLayout);
    }
}

