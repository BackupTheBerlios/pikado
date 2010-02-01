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
#define SCREEN_SINGLE_GLOBAL
#include "screen_single.h"
#include "screen_main.h"
#include "config.h"

void screen_single_create(int bottom_fields)
{
  int i;
  GtkWidget *bottom_table, *top_table;

  if((bottom_fields == 0) || (bottom_fields > SCREEN_SINGLE_BOTTOM_MAX_FIELDS))
  {
    g_message("screen_single_create() Bad fields count !\n");
    gtk_main_quit();
    _exit(0);
  }

  if(GTK_IS_WIDGET(screen_main_vbox))
    gtk_widget_destroy(screen_main_vbox);
  screen_main_vbox = gtk_vbox_new (FALSE, 10);

  top_table = gtk_table_new(config.players, 4, TRUE);
  for(i=0; i<config.players; i++)
  {
    screen_single_player_name [i] = gtk_label_new("");
    screen_single_player_score[i] = gtk_label_new("");

    gtk_misc_set_alignment(GTK_MISC(screen_single_player_name[i]), 0.1, 0.5);
    gtk_widget_modify_font(screen_single_player_name [i], config.label_font);
    gtk_widget_modify_font(screen_single_player_score[i], config.label_font);

    // left, right, top, bottom
    gtk_table_attach(GTK_TABLE(top_table), screen_single_player_name [i], 0, 3, i, i + 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
    gtk_table_attach(GTK_TABLE(top_table), screen_single_player_score[i], 3, 4, i, i + 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
  }
  gtk_container_add(GTK_CONTAINER(screen_main_vbox), top_table);

  bottom_table = gtk_table_new (2, bottom_fields, FALSE);
  for(i=0; i<bottom_fields; i++)
  {
    screen_single_bottom_label[i] = gtk_label_new("");
    screen_single_bottom_value[i] = gtk_label_new("");

    gtk_widget_modify_font(screen_single_bottom_label[i], config.label_font);
    gtk_widget_modify_font(screen_single_bottom_value[i], config.label_font);

    // left, right, top, bottom
    gtk_table_attach(GTK_TABLE(bottom_table), screen_single_bottom_label[i], i, i + 1, 0, 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
    gtk_table_attach(GTK_TABLE(bottom_table), screen_single_bottom_value[i], i, i + 1, 1, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
  }
  gtk_container_add(GTK_CONTAINER(screen_main_vbox), bottom_table);

  gtk_container_add(GTK_CONTAINER(screen_main_wnd), screen_main_vbox);
  gtk_widget_show_all(screen_main_wnd);
}

