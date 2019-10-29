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

#include "output.h"

// Begin Output
void printspecies(struct assignlist * species, struct maplist * map, char * compart)
{
    //DECLARE VALUES
    struct assignlist * specie = species;
    while (specie != NULL) {
        fprintf( yyout, "Specie %s in %s\n", getmap(map, specie->assign->sym->name), compart);
        specie = specie->next;
    }

    //PRINT VALUES
    specie = species;
    while (specie != NULL) {
        fprintf( yyout, "%s = %0.4lf\n", getmap(map, specie->assign->sym->name), specie->assign->sym->value);
        specie = specie->next;
    }
}

void printlocals(struct assignlist* locals, struct maplist * map, char * compart)
{
    //DECLARE VALUES
    struct assignlist * local = locals;
    while (local != NULL) {
        fprintf(yyout, "var %s in %s\n", getmap(map, local->assign->sym->name), compart);
        local = local->next;        
    }

    //PRINT VALUES
    local = locals;
    while (local != NULL) {
        fprintf( yyout, "%s = %0.4lf\n", getmap(map, local->assign->sym->name), local->assign->sym->value);
        local = local->next;
    }
}

void printreaction(struct reaction * reac, struct maplist * map)
{
    struct assignlist * reactant = reac->reactant;
    struct assignlist * product = reac->product;
    int first = 1;
    while (reactant != NULL) {
        if (!first)
            fprintf(yyout, " ");
        else
            first = 0;
        fprintf(yyout, "%s", getmap(map, reactant->assign->sym->name));
        reactant = reactant->next;
    }
    fprintf(yyout, "-> ");
    while (product != NULL) {
        fprintf(yyout, "%s", getmap(map, product->assign->sym->name));

        if(product->next != NULL)
            fprintf(yyout, " ");
        product = product->next;
    }
    fprintf(yyout, "; %s;\n", "RATE");
}

void printreactionlist(struct reactionlist * reactions, struct maplist* map)
{
    struct reactionlist * element = reactions;
    while (element != NULL) {
        printreaction(element->reac, map);
        element = element->next;
    }
}