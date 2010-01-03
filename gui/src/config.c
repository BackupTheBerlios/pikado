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
#include <string.h>

#define CONFIG_GLOBAL
#include "config.h"

#define CFG_FILENAME ".dart/pikado.cfg"

void config_load(void)
{
  int i;
  gchar *filename, *content;
  gsize bytes;
  GError *error = NULL;

  filename = g_build_filename(g_get_home_dir(), CFG_FILENAME, NULL);

  g_file_get_contents(filename, &content, &bytes, &error);
  if(error != NULL)
  {
    g_print("config_load(): Error while loading the configuration file: %s\n", error->message);
    g_clear_error(&error);
    bytes = 0;
  }

  if(bytes != sizeof(Config))
  {
    g_print("config_load(): The configuration file has the wrong size! Creating a new one ...\n");
    memset(&config, 0, sizeof(Config));

    // TODO Replace with the standard widget font ...
    snprintf(config.font_name,         sizeof(config.font_name),         "Sans 10");
    snprintf(config.serial_port,       sizeof(config.serial_port),       "/dev/ttyS0");

    config.players = 4;
    for(i=0; i<MAX_PLAYERS; i++)
      snprintf(config.player_name[i], sizeof(config.player_name[i]), "Player %d", i + 1);

    config_save();
  }
  else
    memcpy(&config, content, sizeof(Config));

  if(config.players > MAX_PLAYERS)
    config.players = 4;
}

void config_save(void)
{
  gchar *filename;
  GError *error = NULL;

  filename = g_build_filename(g_get_home_dir(), CFG_FILENAME, NULL);

  g_file_set_contents(filename, (gchar *) &config, sizeof(Config), &error);
  if(error != NULL)
  {
    g_print("config_save(): Error while saving the configuration file: %s\n", error->message);
    g_clear_error(&error);
  }
}

