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
#include <string.h>
#include <ctype.h>
#define SCREEN_SETTINGS_GLOBAL
#include "screen_settings.h"
#include "screen_main.h"
#include "config.h"

char screen_settings_keyboard_labels_1[][10] = {{"Q"}, {"W"}, {"E"}, {"R"}, {"T"}, {"Z"}, {"U"}, {"I"}, {"O"}, {"P"}, {"Ü"}, {"<--"}, {0}};
char screen_settings_keyboard_labels_2[][10] = {{"A"}, {"S"}, {"D"}, {"F"}, {"G"}, {"H"}, {"J"}, {"K"}, {"L"}, {"Ö"}, {"Ä"}, {0}};
char screen_settings_keyboard_labels_3[][10] = {{"Y"}, {"X"}, {"C"}, {"V"}, {"B"}, {"N"}, {"M"}, {"."}, {"_"}, {"CLR"}, {0}};
int  screen_settings_player_count[MAX_PLAYERS];

/*******************************************************************************
* Create the settings dialog                                                   *
*******************************************************************************/
void screen_settings_create(GtkButton *button, gpointer user_data)
{
  screen_settings_player_names(button, user_data);
}

/*******************************************************************************
* Hightlight the player count buttons                                          *
*******************************************************************************/
void screen_settings_highlight_player_count(void)
{
  int i;
  GdkColor color;

  color.red   = 0;
  color.green = 65535;
  color.blue  = 0;

  for(i=0; i<MAX_PLAYERS; i++)
  {
    gtk_widget_modify_bg(screen_settings_player_button[i], GTK_STATE_NORMAL,   NULL);
    gtk_widget_modify_bg(screen_settings_player_button[i], GTK_STATE_PRELIGHT, NULL);
    gtk_widget_modify_bg(screen_settings_player_button[i], GTK_STATE_ACTIVE,   NULL);
  }

  gtk_widget_modify_bg(screen_settings_player_button[config.players - 1], GTK_STATE_NORMAL,   &color);
  gtk_widget_modify_bg(screen_settings_player_button[config.players - 1], GTK_STATE_PRELIGHT, &color);
  gtk_widget_modify_bg(screen_settings_player_button[config.players - 1], GTK_STATE_ACTIVE,   &color);
}

/*******************************************************************************
* Create a button for the onscreen keyboard                                    *
*******************************************************************************/
void screen_settings_player_names_button(GtkWidget *box, char *label)
{
  GtkWidget *button, *button_label;
  
  button = gtk_button_new_with_label(label);
  button_label = gtk_bin_get_child(GTK_BIN(button));
  gtk_widget_modify_font(button_label, config.label_font);
  gtk_container_add(GTK_CONTAINER(box), button);
  
  g_signal_connect(button, "clicked", 
    G_CALLBACK(screen_settings_player_names_sig_keyboard), 
    label);
}

/*******************************************************************************
* Create the dialog for editing the player names                               *
*******************************************************************************/
void screen_settings_player_names(GtkButton *button, gpointer user_data)
{
  int i;
  char buffer[10];
  GtkWidget *content_area, *vbox, *sep;
  GtkWidget *pbox,*player_label, *button_label;
  GtkWidget *table, *name_label;
  GtkWidget *bbox1, *bbox2, *bbox3;
  GtkWidget *abox, *but_ok, *but_cancel;
  
  screen_settings_player_entry_index = 0;

  // Dialog
  screen_settings_player_names_dialog = gtk_dialog_new();
  gtk_container_set_border_width (GTK_CONTAINER(screen_settings_player_names_dialog), 12);
  gtk_window_set_title(GTK_WINDOW(screen_settings_player_names_dialog), "Pikado: player names");
  gtk_dialog_set_has_separator(GTK_DIALOG(screen_settings_player_names_dialog), FALSE);     
  g_signal_connect_swapped(screen_settings_player_names_dialog, "response", G_CALLBACK (gtk_widget_destroy), screen_settings_player_names_dialog);
  content_area = gtk_dialog_get_content_area(GTK_DIALOG(screen_settings_player_names_dialog));
  vbox = gtk_vbox_new (FALSE, 10);

  // Player count
  pbox = gtk_hbox_new (FALSE, 10);
  gtk_container_add(GTK_CONTAINER(vbox), pbox);
  player_label = gtk_label_new("Players: ");
  gtk_widget_modify_font(player_label, config.label_font);
  gtk_box_pack_start(GTK_BOX(pbox), player_label, FALSE, FALSE, 0);
  for(i=0; i<8; i++)
  {
    sprintf(buffer, "%d", i + 1);
    screen_settings_player_button[i] = gtk_button_new_with_label(buffer);
    button_label = gtk_bin_get_child(GTK_BIN(screen_settings_player_button[i]));
    gtk_widget_modify_font(button_label, config.label_font);
    gtk_container_add(GTK_CONTAINER(pbox), screen_settings_player_button[i]);
    
    screen_settings_player_count[i] = i + 1;
    g_signal_connect(screen_settings_player_button[i], "clicked", 
      G_CALLBACK(screen_settings_player_names_sig_players), 
      &screen_settings_player_count[i]);
  }
  screen_settings_highlight_player_count();
  sep = gtk_hseparator_new();
  gtk_container_add(GTK_CONTAINER(vbox), sep);
  
  // Player names
  table = gtk_table_new(4, 4, FALSE);
  gtk_container_add(GTK_CONTAINER(vbox), table);
  for(i=0; i<4; i++)
  {
    sprintf(buffer, "%d:", i + 1);
    name_label = gtk_label_new(buffer);
    gtk_widget_modify_font(name_label, config.label_font);
    gtk_table_attach(GTK_TABLE(table), name_label, 0, 1, i, i + 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);

    screen_settings_player_entry[i] = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(screen_settings_player_entry[i]), config.player_name[i]);
    gtk_widget_modify_font(screen_settings_player_entry[i], config.label_font);
    gtk_table_attach(GTK_TABLE(table), screen_settings_player_entry[i], 1, 2, i, i + 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);

    sprintf(buffer, "%d:", i + 5);
    name_label = gtk_label_new(buffer);
    gtk_widget_modify_font(name_label, config.label_font);
    gtk_table_attach(GTK_TABLE(table), name_label, 2, 3, i, i + 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);

    screen_settings_player_entry[i + 4] = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(screen_settings_player_entry[i + 4]), config.player_name[i + 4]);
    gtk_widget_modify_font(screen_settings_player_entry[i + 4], config.label_font);
    gtk_table_attach(GTK_TABLE(table), screen_settings_player_entry[i + 4], 3, 4, i, i + 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 0, 0);
  }
  for(i=0; i<MAX_PLAYERS; i++)
  {
    gtk_widget_add_events(screen_settings_player_entry[i], GDK_FOCUS_CHANGE_MASK);
    g_signal_connect(screen_settings_player_entry[i], "focus-in-event", 
      G_CALLBACK(screen_settings_player_names_sig_entry_focus), 
      &screen_settings_player_count[i]);
  }
  sep = gtk_hseparator_new();
  gtk_container_add(GTK_CONTAINER(vbox), sep);

  // Keyboard line 1
  bbox1 = gtk_hbox_new(TRUE, 10);
  gtk_container_add(GTK_CONTAINER(vbox), bbox1);
  for(i=0; i<12; i++)
    screen_settings_player_names_button(bbox1, screen_settings_keyboard_labels_1[i]);

  // Keyboard line 2
  bbox2 = gtk_hbox_new(TRUE, 10);
  gtk_container_add(GTK_CONTAINER(vbox), bbox2);
  for(i=0; i<11; i++)
    screen_settings_player_names_button(bbox2, screen_settings_keyboard_labels_2[i]);

  // Keyboard line 3
  bbox3 = gtk_hbox_new(TRUE, 10);
  gtk_container_add(GTK_CONTAINER(vbox), bbox3);
  for(i=0; i<10; i++)
    screen_settings_player_names_button(bbox3, screen_settings_keyboard_labels_3[i]);

  // Action area
  sep = gtk_hseparator_new();
  gtk_container_add(GTK_CONTAINER(vbox), sep);
  abox = gtk_hbox_new(TRUE, 10);
  gtk_container_add(GTK_CONTAINER(vbox), abox);

  // Cancel button
  but_cancel = gtk_button_new_with_label("Cancel");
  button_label = gtk_bin_get_child(GTK_BIN(but_cancel));
  gtk_widget_modify_font(button_label, config.label_font);
  gtk_container_add(GTK_CONTAINER(abox), but_cancel);
  g_signal_connect(but_cancel, "clicked", 
    G_CALLBACK(screen_settings_player_names_sig_cancel), NULL);
  
  // Ok button
  but_ok = gtk_button_new_with_label("Ok");
  button_label = gtk_bin_get_child(GTK_BIN(but_ok));
  gtk_widget_modify_font(button_label, config.label_font);
  gtk_container_add(GTK_CONTAINER(abox), but_ok);
  g_signal_connect(but_ok, "clicked", 
    G_CALLBACK(screen_settings_player_names_sig_ok), NULL);
  
  gtk_container_add(GTK_CONTAINER(content_area), vbox);
  gtk_widget_show_all(screen_settings_player_names_dialog);
  gtk_dialog_run(GTK_DIALOG(screen_settings_player_names_dialog));
}

/*******************************************************************************
* Signal handler: player count buttons                                         *
*******************************************************************************/
void screen_settings_player_names_sig_players(GtkButton *button, gpointer user_data)
{
  int player_count;

  if(user_data == NULL)
    return;
    
  player_count =  *((int*) user_data);
  config.players = player_count;  
  screen_settings_highlight_player_count();  
}

/*******************************************************************************
* Signal handler: Got focus on player name text entry's                        *
*******************************************************************************/
gboolean screen_settings_player_names_sig_entry_focus(GtkWidget *widget, GdkEventFocus *event, gpointer user_data)
{
  int entry_index;

  if(user_data == NULL)
    return FALSE;

  entry_index = (*((int*) user_data)) - 1;
  screen_settings_player_entry_index = entry_index;
  return FALSE;
}

/*******************************************************************************
* Signal handler: Button's of the onscreen keyboard                            *
*******************************************************************************/
void screen_settings_player_names_sig_keyboard(GtkButton *button, gpointer user_data)
{
  int i;
  char key_text[20];
  char buffer[100];
  
  if(user_data == NULL)
    return;
    
  strcpy(key_text, (char*) user_data);
  strcpy(buffer, gtk_entry_get_text(GTK_ENTRY(screen_settings_player_entry[screen_settings_player_entry_index])));

  if(strcmp(key_text, "CLR") == 0)
  {
    // Clear the complete text
    buffer[0] = 0;
  }
  else if(strcmp(key_text, "<--") == 0)
  {
    // Remove the last character
    if(strlen(buffer) > 0)
      buffer[strlen(buffer) - 1] = 0;
  }
  else
  {
    // Add the character
    if(strlen(buffer) != 0)
    {
      for(i=0; i<strlen(buffer); i++)
        key_text[i] = tolower(key_text[i]);
    }
    strcat(buffer, key_text);
  }

  strcpy(config.player_name[screen_settings_player_entry_index], buffer);
    
  gtk_entry_set_text(GTK_ENTRY(screen_settings_player_entry[screen_settings_player_entry_index]), buffer);
  gtk_window_set_focus(GTK_WINDOW(screen_settings_player_names_dialog), screen_settings_player_entry[screen_settings_player_entry_index]);
  gtk_editable_set_position(GTK_EDITABLE(screen_settings_player_entry[screen_settings_player_entry_index]), strlen(buffer));
}

/*******************************************************************************
* Signal handler: Cancel button                                                *
*******************************************************************************/
void screen_settings_player_names_sig_cancel(GtkButton *button, gpointer user_data)
{
  config_load();
  gtk_dialog_response(GTK_DIALOG(screen_settings_player_names_dialog), -1);
}

/*******************************************************************************
* Signal handler: OK button                                                    *
*******************************************************************************/
void screen_settings_player_names_sig_ok(GtkButton *button, gpointer user_data)
{
  config_save();
  gtk_dialog_response(GTK_DIALOG(screen_settings_player_names_dialog), 0);
}
