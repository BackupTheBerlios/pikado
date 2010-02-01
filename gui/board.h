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
#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include <libconfig.h>

#ifndef BOARD_GLOBAL
  #define BOARD_VAR extern
#else
  #define BOARD_VAR
#endif

#define BOARD_SUCCESS             0
#define BOARD_ERROR              -1
#define BOARD_ERROR_CFG_FILE     -2
#define BOARD_ERROR_SERIAL_PORT  -3

typedef struct _Segment Segment;
struct _Segment
{
  uint8_t segment_nr;
  uint8_t is_double;
  uint8_t is_trible;
  uint8_t number;
  uint8_t score;
};

int8_t  board_connect                           (void);
void    board_disconnect                        (void);
int8_t  board_load_config                       (void);
int     board_load_int_element                  (const config_t *cfg_ptr, const config_setting_t * group, char *element);
void    board_load_config_error                 (config_t *cfg_ptr, char *msg);
int8_t  board_get_segment                       (Segment *segment);
int8_t  board_set_matrix_type                   (void);
void    board_check_matrix_configuration_file   (void);
int8_t  board_get_firmware_version              (char *version, int version_size);

BOARD_VAR long        board_key_next_player;
BOARD_VAR long        board_key_bounce_out;
BOARD_VAR Segment     board_matrix[255];
BOARD_VAR uint8_t     board_recv_buffer[100];
BOARD_VAR char        board_name[255];
BOARD_VAR long        board_output_count;
BOARD_VAR uint8_t     board_is_connected;
BOARD_VAR int         board_fd_ser;

#endif // DART_BOARD_H

