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
#define SCREEN_MAIN_GLOBAL
#include "screen_main.h"
#include "screen_games.h"
#include "screen_single.h"
#include "config.h"

void screen_main_sig_destroy(GtkWidget *widget, gpointer data)
{
  gtk_main_quit();
}

void screen_main_create(void)
{
  screen_main_wnd = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_container_set_border_width (GTK_CONTAINER(screen_main_wnd), 12);
  gtk_window_set_title(GTK_WINDOW(screen_main_wnd), "Pikado");
  gtk_window_set_default_size(GTK_WINDOW(screen_main_wnd), 800, 525);
  g_signal_connect(screen_main_wnd, "destroy", G_CALLBACK(screen_main_sig_destroy), NULL);

  screen_games_create();

  gtk_widget_show_all(screen_main_wnd);
//  gtk_window_maximize(GTK_WINDOW(screen_main_wnd));
}

void screen_main_footer(void)
{
  GtkWidget *hbox, *button, *button_label;
  
  hbox = gtk_hbox_new (FALSE, 10);
  button = gtk_button_new_with_label(">");
  button_label = gtk_bin_get_child(GTK_BIN(button));
  gtk_widget_modify_font(button_label, config.label_font);
  
  gtk_container_add(GTK_CONTAINER(hbox), button);
  gtk_container_add(GTK_CONTAINER(screen_main_vbox), hbox);
}

/*  
  screen_single_create(5);

  gtk_label_set_text(GTK_LABEL(screen_single_bottom_label[0]), "1. Throw");
  gtk_label_set_text(GTK_LABEL(screen_single_bottom_label[1]), "2. Throw");
  gtk_label_set_text(GTK_LABEL(screen_single_bottom_label[2]), "3. Throw");
  gtk_label_set_text(GTK_LABEL(screen_single_bottom_label[3]), "Score");
  gtk_label_set_text(GTK_LABEL(screen_single_bottom_label[4]), "Round");

  gtk_label_set_text(GTK_LABEL(screen_single_bottom_value[0]), "111");
  gtk_label_set_text(GTK_LABEL(screen_single_bottom_value[1]), "222");
  gtk_label_set_text(GTK_LABEL(screen_single_bottom_value[2]), "333");
  gtk_label_set_text(GTK_LABEL(screen_single_bottom_value[3]), "444");
  gtk_label_set_text(GTK_LABEL(screen_single_bottom_value[4]), "555");

  for(i=0; i<config.players; i++)
  {
    gtk_label_set_text(GTK_LABEL(screen_single_player_name [i]), config.player_name[i]);
    gtk_label_set_text(GTK_LABEL(screen_single_player_score[i]), "999");
  }
*/
