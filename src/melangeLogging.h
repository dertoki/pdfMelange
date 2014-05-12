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

#ifndef _MELANGELOGGING_H_
#define _MELANGELOGGING_H_

#include <glib/gstdio.h>

/**
 * \class melangeLogging
 *
 * \brief This is a pseudo class that encapsulates static methods for logging.
 *
 * It's all static, so it's not possible to instantiate the class.
 *
 * Start logging:
 *     melangeLogging::setLogging(true);
 * Do some logging:
 *     g_message("hello World");
 *     bool isLogged = true;
 *     g_message("logging was set to: %s", isLogged ? "true" : "false");
 * End logging:
 *     melangeLogging::setLogging(false);
 *
 */
class melangeLogging
{
public:
    static void setLogging(bool logging);
    static gchar* getLogFileName();

protected:

private:
    static void LogToFile (const gchar *log_domain, GLogLevelFlags log_level, const gchar *message, gpointer user_data);
    static void LogToNull (const gchar *log_domain, GLogLevelFlags log_level, const gchar *message, gpointer user_data );

};

#endif // _MELANGELOGGING_H_
