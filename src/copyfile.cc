// copyfile.cc
//
// Copyright (C) 2015 - Tobias
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "copyfile.h"

#ifdef __WIN32
    #include <windows.h>
    #include <tchar.h>
    #include <stdio.h>
    // see http://sourceforge.net/p/mingw/bugs/666/
    #undef tmpfile
    FILE * tmpfile(void){
        char filename[MAX_PATH];
        char tchTmpPath[MAX_PATH];
        FILE *f;

        GetTempPath(MAX_PATH, tchTmpPath);
        GetTempFileName( tchTmpPath, "pdfmelange", 1, filename);
        f = fopen(filename,"w+bTD");
        return f;
    }

    int copyfile(FILE* inFile, FILE* outFile){
        char buf[4096]; // 4K bock buffer
        ssize_t countIn, countOut; // unsigned long int
        while (countIn = fread(buf, sizeof(char), sizeof(buf), inFile)) {
            countOut = fwrite(buf, sizeof(char), countIn, outFile);
            if (countOut != countIn)
                return 0;
        }
        return 1;
    }
#else
    #include <sys/stat.h>
    #include <sys/sendfile.h>

    int copyfile(FILE* inFile, FILE* outFile){
        struct stat sb;
        fstat(fileno(inFile), &sb); // get information about the attributes of tempFile.
        if (sendfile(fileno(outFile), fileno(inFile), NULL, sb.st_size) < 0)
            return 0;
        else
            return 1;
    }
#endif
