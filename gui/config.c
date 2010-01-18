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
#include <glib.h>
#include <glib/gstdio.h>
#include <string.h>
#include <unistd.h>

#define CONFIG_GLOBAL
#include "config.h"

#define CFG_DIRNAME  ".pikado"
#define CFG_FILENAME ".pikado/config"

// Load the configuration from the file
void config_load(void)
{
  int i;
  gchar *filename;
  struct config_t cfg;
  char setting_name[100], setting_init_value[100];

  config_check_config_directory();
  filename = g_build_filename(g_get_home_dir(), CFG_FILENAME, NULL);
  config_init(&cfg);
  config_read_file(&cfg, filename);

  config_load_string_value(&cfg, "font",        config.font_name,   sizeof(config.font_name),   "Sans 10");
  config.label_font = pango_font_description_from_string(config.font_name);
  
  config_load_string_value(&cfg, "serial_port", config.serial_port, sizeof(config.serial_port), "/dev/ttyS0");
  config_load_int_value(   &cfg, "players",     &config.players,                                4);

  for(i=0; i<MAX_PLAYERS; i++)
  {
    snprintf(setting_name,       sizeof(setting_name),       "player_name_%d", i + 1);
    snprintf(setting_init_value, sizeof(setting_init_value), "Player %d",      i + 1);
    config_load_string_value(&cfg, setting_name, config.player_name[i], sizeof(config.player_name[i]), setting_init_value);
  }
  
  config_write_file(&cfg, filename);
  config_destroy(&cfg);
}

// Save the configuration to the file
void config_save(void)
{
  int i;
  gchar *filename;
  struct config_t cfg;
  char setting_name[100], setting_init_value[100];

  filename = g_build_filename(g_get_home_dir(), CFG_FILENAME, NULL);

  config_init(&cfg);
  config_read_file(&cfg, filename);

  config_save_string_value(&cfg, "font",        config.font_name);
  config_save_string_value(&cfg, "serial_port", config.serial_port);
  config_save_int_value(   &cfg, "players",     &config.players);

  for(i=0; i<MAX_PLAYERS; i++)
  {
    snprintf(setting_name,       sizeof(setting_name),       "player_name_%d", i + 1);
    snprintf(setting_init_value, sizeof(setting_init_value), "Player %d",      i + 1);
    config_save_string_value(&cfg, setting_name, config.player_name[i]);
  }
  
  config_write_file(&cfg, filename);
  config_destroy(&cfg);
}

// Check the directory for the configuration files.
// Create a new directory if it don't exist.
void config_check_config_directory(void)
{
  gchar *filename;
  struct stat stat_buf;

  filename = g_build_filename(g_get_home_dir(), CFG_DIRNAME, NULL);
  if(g_stat(filename, &stat_buf) != 0)
  {
    if(g_mkdir(filename, 0777) != 0)
    {
      g_print("config_load(): Error while creating the configuration directory\n");
      _exit(-1);
    }
  }
}

// Load a "long" value from the configuration file.
// If the setting don't exist, it si created with an inital value.
void config_load_int_value(const config_t *cfg_ptr, const char *path, long *value, int init_value)
{
  if(!config_lookup_int(cfg_ptr, path, value))
  {
    config_setting_t *setting = NULL;
    
    setting = config_setting_add(cfg_ptr->root, path, CONFIG_TYPE_INT);
    *value = init_value;
    config_setting_set_int(setting, *value);
  }
}

// Saves a "long" value from the configuration file
void config_save_int_value(const config_t *cfg_ptr, const char *path, long *value)
{
  config_setting_t *setting = NULL;

  setting = config_lookup(cfg_ptr, path);
  if(setting != NULL)
    config_setting_set_int(setting, *value);
}

// Load a "string" value from the configuration file.
// If the setting don't exist, it si created with an inital value.
void config_load_string_value(const config_t *cfg_ptr, const char *path, char *value, int value_size, char *init_value)
{
  const char *setting_ptr;

  if(!config_lookup_string(cfg_ptr, path, &setting_ptr))
  {
    config_setting_t *setting = NULL;
    
    setting = config_setting_add(cfg_ptr->root, path, CONFIG_TYPE_STRING);
    strncpy(value, init_value, value_size);
    config_setting_set_string(setting, value);
  }
  else
    strncpy(value, setting_ptr, value_size);
}

// Saves a "string" value from the configuration file.
// If the setting don't exist, it si created with an inital value.
void config_save_string_value(const config_t *cfg_ptr, const char *path, char *value)
{
  config_setting_t *setting = NULL;

  setting = config_lookup(cfg_ptr, path);
  if(setting != NULL)
    config_setting_set_string(setting, value);
}
