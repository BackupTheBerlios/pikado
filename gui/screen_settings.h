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
#ifndef SCREEN_SETTINGS_H
#define SCREEN_SETTINGS_H

#include <gtk/gtk.h>
#include "config.h"

#ifndef SCREEN_SETTINGS_GLOBAL
  #define SCREEN_SETTINGS_VAR extern
#else
  #define SCREEN_SETTINGS_VAR
#endif

SCREEN_SETTINGS_VAR GtkWidget *screen_settings_player_button[8];
SCREEN_SETTINGS_VAR GtkWidget *screen_settings_player_entry[8];

void screen_settings_create(GtkButton *button, gpointer user_data);
void screen_settings_player_names(GtkButton *button, gpointer user_data);
void screen_settings_player_names_button(GtkWidget *box, char *label);
void screen_settings_highlight_player_count(void);

#endif // SCREEN_SETTINGS_H

