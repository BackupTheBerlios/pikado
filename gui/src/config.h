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
#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#ifndef CONFIG_GLOBAL
  #define CONFIG_VAR extern
#else
  #define CONFIG_VAR
#endif

#define MAX_PLAYERS 8

typedef struct _Config Config;
struct _Config
{
  char     font_name[100];
  char     serial_port[100];
  uint8_t  players;
  char     player_name[MAX_PLAYERS][100];
};

CONFIG_VAR Config config;

void config_load(void);
void config_save(void);

#endif // CONFIG_H

