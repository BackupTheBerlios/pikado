/*******************************************************************************
*  Pikado - Interface electronic and GUI for a PC based e-dart machine         *
*  Copyright (C) 2006 - 2010 Matthias Klein <matthias.klein@linux.com>         *
*                                                                              *
*  This file is part of Pikado.                                                *
*                                                                              *
*  Pikado is free software: you can redistribute it and/or modify              *
*  it under the terms of the GNU General Public License as published by        *
*  the Free Software Foundation, either version 3 of the License, or           *
*  (at your option) any later version.                                         *
*                                                                              *
*  Pikado is distributed in the hope that it will be useful,                   *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of              *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
*  GNU General Public License for more details.                                *
*                                                                              *
*  You should have received a copy of the GNU General Public License           *
*  along with Pikado.  If not, see <http://www.gnu.org/licenses/>.             *
*******************************************************************************/
#ifndef SCREEN_SINGLE_H
#define SCREEN_SINGLE_H

#include <gtk/gtk.h>
#include "config.h"

#ifndef SCREEN_SINGLE_GLOBAL
  #define SCREEN_SINGLE_VAR extern
#else
  #define SCREEN_SINGLE_VAR
#endif

#define SCREEN_SINGLE_BOTTOM_MAX_FIELDS  10

SCREEN_SINGLE_VAR GtkWidget *screen_single_player_name [MAX_PLAYERS];
SCREEN_SINGLE_VAR GtkWidget *screen_single_player_score[MAX_PLAYERS];
SCREEN_SINGLE_VAR GtkWidget *screen_single_bottom_label[SCREEN_SINGLE_BOTTOM_MAX_FIELDS];
SCREEN_SINGLE_VAR GtkWidget *screen_single_bottom_value[SCREEN_SINGLE_BOTTOM_MAX_FIELDS];

void screen_single_create(int bottom_fields);

#endif // SCREEN_SINGLE_H

