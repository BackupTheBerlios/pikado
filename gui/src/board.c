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
#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BOARD_GLOBAL
#include "board.h"
#include "config.h"

#define BOARD_FILENAME ".dart/pikado.dbc"

int board_load_config(void)
{
  int i;
  FILE *fd;
  char line[255];
  gchar *filename;

  char *start, *eq_sign, *next_comma;
  unsigned char segment_nr;

  memset(board_matrix, 0, sizeof(board_matrix));
  for(i=0; i<256; i++)
    board_matrix[i].segment_nr = i;

  board_key_next_player = 255;
  board_key_bounce_out  = 254;
  board_is_connected    = FALSE;

  filename = g_build_filename(g_get_home_dir(), BOARD_FILENAME, NULL);
  fd = fopen(filename, "r");

  if(fd != NULL)
  {
    while(fgets(line, sizeof(line), fd) != NULL)
    {
      start = line;

      if(strstr(line, "//") != NULL)
      {
      }
      else if((eq_sign = strchr(line, '=')) != NULL)
      {
        *eq_sign = 0;
        if(strcmp(start, "Name") == 0)
        {
          start = eq_sign + 1;
          eq_sign = strchr(start, '\n');
          *eq_sign = 0;
          strcpy(board_name, start);
        }
        else if(strcmp(start, "Outputs") == 0)
        {
          eq_sign++;
          board_output_count = (unsigned char) atoi(eq_sign);
        }
        else if(strcmp(start, "Key_Next_Player") == 0)
        {
          eq_sign++;
          board_key_next_player = (unsigned char) atoi(eq_sign);
        }
        else if(strcmp(start, "Key_Bounce_Out") == 0)
        {
          eq_sign++;
          board_key_bounce_out = (unsigned char) atoi(eq_sign);
        }
      }
      else if((next_comma = strchr(line, ',')) != NULL)
      {
        *next_comma = 0;
        segment_nr = (unsigned char) atoi(start);
        start = next_comma + 1;

        next_comma = strchr(start, ',');
        *next_comma = 0;
        if(atoi(start) != 0)
          board_matrix[segment_nr].is_double = TRUE;
        else
          board_matrix[segment_nr].is_double = FALSE;
        start =next_comma + 1;

        next_comma = strchr(start, ',');
        *next_comma = 0;
        if(atoi(start) != 0)
          board_matrix[segment_nr].is_trible = TRUE;
        else
          board_matrix[segment_nr].is_trible = FALSE;
        start = next_comma + 1;

        board_matrix[segment_nr].number = (unsigned char) atoi(start);

        if(board_matrix[segment_nr].is_double)
          board_matrix[segment_nr].score = board_matrix[segment_nr].number * 2;
        else if(board_matrix[segment_nr].is_trible)
          board_matrix[segment_nr].score = board_matrix[segment_nr].number * 3;
        else
          board_matrix[segment_nr].score = board_matrix[segment_nr].number;
      }
    }
  }
  else
    return BOARD_ERROR;

  fclose(fd);


  return BOARD_SUCCESS;
}

int8_t board_connect(void)
{
  int flag_true = 1;
  struct termios termios_p;

  board_is_connected = FALSE;
  close(board_fd_ser);

  if(board_load_config() != BOARD_SUCCESS)
    return BOARD_ERROR_CFG_FILE;

  if((board_fd_ser = open(config.serial_port, O_RDWR | O_NOCTTY | O_NDELAY)) == -1)
    return BOARD_ERROR;

  if(ioctl(board_fd_ser, FIONBIO, &flag_true) == -1)
  {
    close(board_fd_ser);
    return BOARD_ERROR;
  }

  if((tcgetattr(board_fd_ser, &termios_p)) != 0)
  {
    close(board_fd_ser);
    return BOARD_ERROR;
  }

  // 19200 Baud
  cfsetispeed(&termios_p, B19200);
  cfsetospeed(&termios_p, B19200);

  // 8N1
  termios_p.c_cflag &= ~PARENB;
  termios_p.c_cflag &= ~CSTOPB;
  termios_p.c_cflag &= ~CSIZE;
  termios_p.c_cflag |= CS8;

  // Raw input + output
  termios_p.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  termios_p.c_oflag &= ~OPOST;
  cfmakeraw(&termios_p);

  if((tcsetattr(board_fd_ser, TCSANOW, &termios_p)) != 0)
  {
    close(board_fd_ser);
    return BOARD_ERROR;
  }

  if(tcflush(board_fd_ser, TCIFLUSH) == -1)
  {
    close(board_fd_ser);
    return BOARD_ERROR;
  }

  if(board_set_matrix_type() != BOARD_SUCCESS)
    return BOARD_ERROR;

  board_is_connected = TRUE;

  return BOARD_SUCCESS;
}

void board_disconnect(void)
{
  close(board_fd_ser);
  board_is_connected = FALSE;
}

int8_t board_get_segment(Segment *segment)
{

  int recv_len;
  unsigned char segment_nr;

  recv_len = read(board_fd_ser, &board_recv_buffer , 1);

  if(recv_len == -1)
  {
    if(errno != EAGAIN)
    {
      close(board_fd_ser);
      return BOARD_ERROR;
    }
  }
  else
  {
    if(recv_len > 0)
    {
      // TODO: Pufferung von mehreren Pfeilen
      segment_nr = board_recv_buffer[0];

      memcpy(segment, &board_matrix[segment_nr], sizeof(Segment));
      return 1;
    }
  }

  return 0;
}

int8_t board_set_matrix_type(void)
{

  int recv_len;
  unsigned char retry = 10;

  if((board_output_count < 4) || (board_output_count > 8))
    return BOARD_ERROR;

  if(write(board_fd_ser, &board_output_count, 1) == -1)
    return BOARD_ERROR;

  while(retry)
  {
    recv_len = read(board_fd_ser, &board_recv_buffer , 1);

    if(recv_len == -1)
    {
      if(errno != EAGAIN)
        return BOARD_ERROR;
    }
    else
    {
      if(recv_len > 0)
      {
        if(board_recv_buffer[0] == 0x00)
        {
          return BOARD_SUCCESS;
        }
        else
        {
          close(board_fd_ser);
          return BOARD_ERROR;
        }
      }
    }

    usleep(100000);
    retry--;
  }

  return BOARD_ERROR;
}

