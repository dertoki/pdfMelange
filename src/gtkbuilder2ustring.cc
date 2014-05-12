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

#include <iostream>
#include <fstream>
#include <string.h>

// swap char* a_str, char* b_str
inline void swap(char** a_str, char** b_str)
{
    char* temp_str;
    temp_str = *b_str;
    *b_str = *a_str;
    *a_str = temp_str;
}

#define MAX_CHAR 500
void ui_convert(std::ifstream* is, std::ostream* os)
{
    if (!*is) return;
    *os << "static Glib::ustring ui_builder =" << std::endl;

    char *c1str = new char[MAX_CHAR+1];
    char *c2str = new char[MAX_CHAR+1];

    is->getline(c1str,MAX_CHAR);
    swap(&c1str, &c2str);
    while (is->getline(c1str,MAX_CHAR))
    {
        for (int i = 0; i < static_cast<int>(strlen(c2str)); i++)
            if (c2str[i] == '\"')
                c2str[i] = '\'';
        *os << "\"" << c2str << "\"" << std::endl;
        swap(&c1str, &c2str);
    }
    for (int i = 0; i < static_cast<int>(strlen(c2str)); i++)
        if (c2str[i] == '\"')
            c2str[i] = '\'';
    *os << "\"" << c2str << "\";" << std::endl;

    delete[] c1str;
    delete[] c2str;
}

int main(int argc, char *argv[])
{

    switch (argc)
    {
    case 1:
    {
        std::cout << "Convert GtkBuilder UI-file to Gtk::ustring" << std::endl;
        std::cout << "<inputfile> [<outputfile>]" << std::endl;
        break;
    }
    case 2:
    {
        std::ifstream is(argv[1]);
        ui_convert(&is, &std::cout);
        break;
    }
    case 3:
    {
        std::ifstream is(argv[1]);
        std::ofstream os(argv[2]);
        ui_convert(&is, &os);
        break;
    }
    default:
    {
        break;
    }
    }
    return 0;
}
