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
#include <gtk/gtk.h>
#include <unistd.h>
#include <string.h>
#include "config.h"
#include "board.h"

long hit_counter = 0;
GtkWidget *val_matrix_value, *val_hit_counter, *val_segment;

gboolean sig_timer(gpointer data)
{
  char buffer[30];
  char segment_name[4];
  Segment segment;

  if(board_get_segment(&segment))
  {
    sprintf(buffer, "%d", segment.segment_nr);
    gtk_label_set_text(GTK_LABEL(val_matrix_value), buffer);

    if(segment.number != 0)
    {
      if(segment.number == 25)
        strcpy(segment_name, "BE");
      else
        sprintf(segment_name, "%d", segment.number);

      if(segment.is_double)
        sprintf(buffer, "D%s", segment_name);
      else if(segment.is_trible)
        sprintf(buffer, "T%s", segment_name);
      else
        sprintf(buffer, "%s", segment_name);
    }
    else
      sprintf(buffer, "---");

    gtk_label_set_text(GTK_LABEL(val_segment), buffer);

    hit_counter++;
    sprintf(buffer, "%ld", hit_counter);
    gtk_label_set_text(GTK_LABEL(val_hit_counter), buffer);
  }

  return TRUE;
}

void sig_destroy(GtkWidget *widget, gpointer data)
{
  gtk_main_quit();
}

int main(int argc, char **argv)
{
  GtkWidget *main_wnd, *vbox, *table;
  GtkWidget *lbl_matrix_value, *lbl_hit_counter, *lbl_segment, *lbl_firmware, *val_firmware;
  PangoFontDescription *label_font;
  char firmware_version[100];
  
  gtk_init(&argc, &argv);
  config_load();

  if(board_connect() != BOARD_SUCCESS)
  {
    GtkWidget *dialog;

    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
      GTK_BUTTONS_OK, "Connection to pikado interface failed !");
    gtk_window_set_title(GTK_WINDOW(dialog), "Pikado Diag");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    _exit(-1);
  }

  if(board_get_firmware_version(firmware_version, sizeof(firmware_version)) != BOARD_SUCCESS)
  {
    GtkWidget *dialog;

    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
      GTK_BUTTONS_OK, "Reading firmware version from interface failed !");
    gtk_window_set_title(GTK_WINDOW(dialog), "Pikado Diag");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    _exit(-1);
  }

  label_font = pango_font_description_from_string(config.font_name);

  main_wnd = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_container_set_border_width (GTK_CONTAINER(main_wnd), 12);
  gtk_window_set_title(GTK_WINDOW(main_wnd), "Pikado Diag");
  gtk_window_set_position(GTK_WINDOW(main_wnd), GTK_WIN_POS_CENTER);
  g_signal_connect(main_wnd, "destroy", G_CALLBACK(sig_destroy), NULL);
  vbox = gtk_vbox_new (FALSE, 10);

  table = gtk_table_new(4, 2, TRUE);
  gtk_table_set_row_spacings(GTK_TABLE(table), 10);
  gtk_table_set_col_spacings(GTK_TABLE(table), 10);
  gtk_container_add(GTK_CONTAINER(vbox), table);

  lbl_matrix_value = gtk_label_new("Matrix value:");
  gtk_widget_modify_font(lbl_matrix_value, label_font);
  gtk_misc_set_alignment(GTK_MISC(lbl_matrix_value), 0, 0.5);

  val_matrix_value = gtk_label_new("---");
  gtk_widget_modify_font(val_matrix_value, label_font);

  lbl_hit_counter = gtk_label_new("Hit counter:");
  gtk_widget_modify_font(lbl_hit_counter, label_font);
  gtk_misc_set_alignment(GTK_MISC(lbl_hit_counter), 0, 0.5);

  val_hit_counter = gtk_label_new("0");
  gtk_widget_modify_font(val_hit_counter, label_font);

  lbl_segment = gtk_label_new("Segment:");
  gtk_widget_modify_font(lbl_segment, label_font);
  gtk_misc_set_alignment(GTK_MISC(lbl_segment), 0, 0.5);

  val_segment = gtk_label_new("---");
  gtk_widget_modify_font(val_segment, label_font);

  lbl_firmware = gtk_label_new("Firmware version:");
  gtk_widget_modify_font(lbl_firmware, label_font);
  gtk_misc_set_alignment(GTK_MISC(lbl_firmware), 0, 0.5);

  val_firmware = gtk_label_new("");
  gtk_widget_modify_font(val_firmware, label_font);

  // left, right, top, bottom
  gtk_table_attach(GTK_TABLE(table), lbl_matrix_value, 0, 1, 0, 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
  gtk_table_attach(GTK_TABLE(table), val_matrix_value, 1, 2, 0, 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
  gtk_table_attach(GTK_TABLE(table), lbl_hit_counter,  0, 1, 1, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
  gtk_table_attach(GTK_TABLE(table), val_hit_counter,  1, 2, 1, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
  gtk_table_attach(GTK_TABLE(table), lbl_segment,      0, 1, 2, 3, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
  gtk_table_attach(GTK_TABLE(table), val_segment,      1, 2, 2, 3, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
  gtk_table_attach(GTK_TABLE(table), lbl_firmware,     0, 1, 3, 4, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
  gtk_table_attach(GTK_TABLE(table), val_firmware,     1, 2, 3, 4, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);

  gtk_label_set_text(GTK_LABEL(val_firmware), firmware_version);

  g_timeout_add(100, sig_timer, NULL);

  gtk_container_add(GTK_CONTAINER(main_wnd), vbox);
  gtk_widget_show_all(main_wnd);
  gtk_main();

  return 0;
}
