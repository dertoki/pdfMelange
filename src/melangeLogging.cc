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

#include "melangeLogging.h"

#define LOGFILENAME "pdfMelange.log"

#ifdef __WIN32
#define GETTEXT_PACKAGE "pdfmelange"
#define PROGRAMNAME_LOCALEDIR "../share/locale"
#else
#include "config.h"
#endif


/**
 * \brief Get the logging file name.
 *
 * Create the a filename in the user home directory and return it.
 *
 * \return gchar*
 */
gchar* melangeLogging::getLogFileName()
{
    gchar *logfilename = g_build_filename (g_get_home_dir(), LOGFILENAME, NULL);
    return logfilename;
}

/**
 * \brief Enable or disable logging.
 *
 * When enabled, this removes the old logfile and creates a new one with a basic header.
 *
 * \param bool logging
 * \return void
 */
void melangeLogging::setLogging (bool logging)
{
    if (logging)
    {
        /** remove old logfile */
        gchar* logfilename = melangeLogging::getLogFileName();
        if ( logfilename ) g_remove( logfilename );

        /** Send g_message(  ) and g_warning(  ) to the file */
        g_log_set_handler (NULL, G_LOG_LEVEL_MESSAGE, melangeLogging::LogToFile, NULL);

        /** do some logging */
        gchar *now_str = g_date_time_format(g_date_time_new_now_local(), "%F - %T");
        g_message("------------------");
        g_message("pdfMelange logfile created at %s", now_str);
        g_message("------------------");
        g_free(now_str);

        g_message("          log file: %s", logfilename);
        g_message("   gettext package: %s", GETTEXT_PACKAGE);
        g_message("        locale dir: %s", PROGRAMNAME_LOCALEDIR);
        g_free(logfilename);
    }
    else
    {
        g_log_set_handler(NULL, G_LOG_LEVEL_MESSAGE, melangeLogging::LogToNull, NULL);
    }
}

/**
 * \brief Manage logging to a file
 */
void melangeLogging::LogToFile (const gchar *log_domain, GLogLevelFlags log_level, const gchar *message, gpointer user_data)
{
    gchar* logfilename = melangeLogging::getLogFileName();
    FILE *logfile = fopen (logfilename, "a");
    g_free(logfilename);
    if (logfile == NULL)
    {
        /* Fall back to console output if unable to open file */
        printf ("Rerouted to console: %s\n", message);
        return;
    }

    fprintf (logfile, "%s\n", message);
    fclose (logfile);
}

/**
 * \brief Manage disable logging
 */
void melangeLogging::LogToNull(const gchar *log_domain, GLogLevelFlags log_level, const gchar *message, gpointer user_data )
{
    return ;
}
