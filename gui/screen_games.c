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
#include <unistd.h>
#define SCREEN_GAMES_GLOBAL
#include "screen_games.h"
#include "screen_main.h"
#include "config.h"

void screen_games_create(void)
{
  GtkWidget *button, *button_label;

  if(GTK_IS_WIDGET(screen_main_vbox))
    gtk_widget_destroy(screen_main_vbox);
  screen_main_vbox = gtk_vbox_new (FALSE, 10);

  button = gtk_button_new_with_label("301");
  button_label = gtk_bin_get_child(GTK_BIN(button));
  gtk_widget_modify_font(button_label, config.label_font);
  gtk_container_add(GTK_CONTAINER(screen_main_vbox), button);

  gtk_container_add(GTK_CONTAINER(screen_main_wnd), screen_main_vbox);
  gtk_widget_show_all(screen_main_wnd);
}
