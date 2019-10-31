/*
 *    Copyright (C) 2019 Alek Frohlich <alek.frohlich@gmail.com>
 *    & Gustavo Biage <gustavo.c.biage@gmail.com>.
 *
 * 	  This file is a part of Albi.
 *
 *    Albi is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    Albi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License along
 *    with this program; if not, write to the Free Software Foundation, Inc.,
 *    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdio.h>

#include "albi.tab.h"

int nowrites;           // Symbol table writable?
int yycolumn;           // Bison location
char *currfilename;     // Current yyin file name

/**
 * Display error message and exit
 */
int yyerror(const char *s)
{
    #define ERROR_RED(s)    fprintf(stderr, "\033[1;31m%s\033[0m", s)
    #define BEGIN_BOLD      fprintf(stderr, "\033[1;80m")
    #define COLOR_RESET     fprintf(stderr, "\033[0m")

    if (yylloc.first_line)
    {
        BEGIN_BOLD;
        fprintf(stderr, "%s:%d:%d: ", currfilename, yylloc.last_line,
                yylloc.last_column);
        COLOR_RESET;
    }

    ERROR_RED("error: ");
    fprintf(stderr, "%s.\n", s);
}