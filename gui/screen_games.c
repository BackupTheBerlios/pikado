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

void screen_games_button(char *label, int x, int y, GCallback func)
{
  GtkWidget *button, *button_label;
  
  button = gtk_button_new_with_label(label);
  button_label = gtk_bin_get_child(GTK_BIN(button));
  gtk_widget_modify_font(button_label, config.label_font);
  gtk_label_set_justify(GTK_LABEL(button_label), GTK_JUSTIFY_CENTER);

  // left, right, top, bottom
  gtk_table_attach(GTK_TABLE(screen_games_table), button, x, x+1, y, y+1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
  if(func != NULL)
    gtk_signal_connect(GTK_OBJECT (button), "clicked", GTK_SIGNAL_FUNC(func), NULL);
}

void screen_games_create(void)
{
  if(GTK_IS_WIDGET(screen_main_vbox))
    gtk_widget_destroy(screen_main_vbox);
  screen_main_vbox = gtk_vbox_new (FALSE, 10);

  screen_games_table = gtk_table_new(3, 3, TRUE);

  screen_games_button("301 / 501 / 701",      0, 0, NULL);
  screen_games_button("Round\nthe clock",     1, 0, NULL);
  screen_games_button("Cricket",              2, 0, GTK_SIGNAL_FUNC(screen_games_sig_cricket));

  screen_games_button("Splitscore",           0, 1, NULL);
  screen_games_button("Segment\ntraining",    1, 1, NULL);
  screen_games_button("",                     2, 1, NULL);

  screen_games_button("",                     0, 2, NULL);
  screen_games_button("",                     1, 2, NULL);
  screen_games_button("",                     2, 2, NULL);

  gtk_container_add(GTK_CONTAINER(screen_main_vbox), screen_games_table);
  screen_main_footer();
  gtk_container_add(GTK_CONTAINER(screen_main_wnd), screen_main_vbox);
  gtk_widget_show_all(screen_main_wnd);
}

void screen_games_sig_cricket(GtkWidget *widget, gpointer data)
{
  g_print("cricket\n");
}
