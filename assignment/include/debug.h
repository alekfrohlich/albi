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

#ifdef DEBUG
#define DEBUG1                                                                 \
  for (struct reactionlist *it = program->reactions; it != NULL;               \
       it = it->next) {                                                        \
    struct reaction *reac = it->reac;                                          \
    printf("\nREACTION\n");                                                    \
    for (struct nodelist *it2 = reac->reactant; it2 != NULL;                   \
         it2 = it2->next) {                                                    \
      printf("\nREACT %s\n", ((struct symassign *)it2->node)->sym->name);      \
      errexp(((struct symassign *)it2->node)->val);                            \
    }                                                                          \
    for (struct nodelist *it3 = reac->product; it3 != NULL; it3 = it3->next) { \
      printf("\nPROD %s\n", ((struct symassign *)it3->node)->sym->name);       \
      errexp(((struct symassign *)it3->node)->val);                            \
    }                                                                          \
  }
#else
#define DEBUG1
#endif