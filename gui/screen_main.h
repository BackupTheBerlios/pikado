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
#ifndef SCREEN_MAIN_H
#define SCREEN_MAIN_H

#include <gtk/gtk.h>

#ifndef SCREEN_MAIN_GLOBAL
  #define SCREEN_MAIN_VAR extern
#else
  #define SCREEN_MAIN_VAR
#endif

SCREEN_MAIN_VAR GtkWidget *screen_main_wnd, *screen_main_vbox;

void screen_main_sig_destroy(GtkWidget *widget, gpointer data);
void screen_main_create(void);
void screen_main_footer(void);

#endif // SCREEN_MAIN_H

