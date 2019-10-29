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

#ifndef OUTPUT_H
#define OUTPUT_H

    #include "program.h"
    #include "structures.h"

    // Forward definitions.
    extern FILE * yyout;
    extern void printspecies(struct nodelist * species, struct maplist * map, char * compart);
    extern void printlocals(struct nodelist* locals, struct maplist * map, char * compart);
    extern void printreaction(struct reaction * reac, struct maplist * map);
    extern void printreactionlist(struct reactionlist * reactions, struct maplist* map);

#endif  // OUTPUT_H