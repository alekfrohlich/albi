/*
 *    Copyright (C) 2019 Alek Frohlich <alek.frohlich@gmail.com>
 *    & Gustavo Biage <gustavo.c.biage@gmail.com>.
 *
 *       This file is a part of Albi.
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

#include <stdlib.h>
#include <string.h>

#include "structures.h"

/**
 * Create map
 */
struct maplist *newmaplist(char *key, char *value, struct maplist *next) {
  struct maplist *ml = (struct maplist *)malloc(sizeof(struct maplist));
  ml->key = key;
  ml->value = value;
  ml->next = next;
  return ml;
}

/**
 * Get entry on map
 */
char *getmap(struct maplist *map, char *key) {
  if (map == NULL)
    return NULL;
  if (strcmp(map->key, key) == 0)
    return map->value;
  return getmap(map->next, key);
}