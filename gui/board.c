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

#define BOARD_FILENAME ".pikado/matrix"

/*******************************************************************************
* Load the matrix configuration                                                *
*******************************************************************************/
int8_t board_load_config(void)
{
  int i, config_array_count, segment_nr, number;
  gchar *filename;
  char err_msg[255];
  struct config_t cfg;
  config_setting_t *matrix_root = NULL;
  config_setting_t *matrix_group = NULL;

  memset(board_matrix, 0, sizeof(board_matrix));
  for(i=0; i<256; i++)
    board_matrix[i].segment_nr = i;

  board_check_matrix_configuration_file();

  filename = g_build_filename(g_get_home_dir(), BOARD_FILENAME, NULL);
  config_init(&cfg);
  if(!config_read_file(&cfg, filename))
  {
    snprintf(err_msg, sizeof(err_msg), "board_load_config(): %s in line %d while parsing matrix configuration !\n", config_error_text(&cfg), config_error_line(&cfg));
    board_load_config_error(&cfg, err_msg);
  }

  // General settings
  if(!config_lookup_int(&cfg, "output_count", &board_output_count))
    board_load_config_error(&cfg, "board_load_config(): Failed to lookup \"output_count\" !\n");

  if(!config_lookup_int(&cfg, "key_next_player", &board_key_next_player))
    board_load_config_error(&cfg, "board_load_config(): Failed to lookup \"key_next_player\" !\n");

  if(!config_lookup_int(&cfg, "key_bounce_out", &board_key_bounce_out))
    board_load_config_error(&cfg, "board_load_config(): Failed to lookup \"key_bounce_out\" !\n");

  // Matrix group array
  matrix_root = config_lookup(&cfg, "matrix");
  if(matrix_root == NULL)
    board_load_config_error(&cfg, "board_load_config(): No \"matrix\" element in configuration file !\n");
  
  config_array_count = config_setting_length(matrix_root);
  if(config_array_count == 0)
    board_load_config_error(&cfg, "board_load_config(): The \"matrix\" element in not an array !\n");
  
  for(i=0; i<config_array_count; i++)
  {
    matrix_group = config_setting_get_elem(matrix_root, i);

    if(matrix_group == NULL)
      board_load_config_error(&cfg, "board_load_config(): Failed to lookup index inside the \"matrix\" array !\n");
    
    // Segment number 1..20 / bull
    number = board_load_int_element(&cfg, matrix_group, "segment");

    // Single
    segment_nr = board_load_int_element(&cfg, matrix_group, "single");
    board_matrix[segment_nr].number = number;
    board_matrix[segment_nr].score  = number;
    
    // Double
    segment_nr = board_load_int_element(&cfg, matrix_group, "double");
    board_matrix[segment_nr].number = number;
    board_matrix[segment_nr].score  = number * 2;
    board_matrix[segment_nr].is_double = TRUE;
    
    // Trible
    segment_nr = board_load_int_element(&cfg, matrix_group, "trible");
    board_matrix[segment_nr].number = number;
    board_matrix[segment_nr].score  = number * 3;
    board_matrix[segment_nr].is_trible = TRUE;
  }

  config_destroy(&cfg);

  return BOARD_SUCCESS;
}

/*******************************************************************************
* Exit the application with an error message                                   *
*******************************************************************************/
void board_load_config_error(config_t *cfg_ptr, char *msg)
{
  g_print("%s", msg);
  config_destroy(cfg_ptr);
  exit(-1);
}

/*******************************************************************************
* Load a named member from the configuration array                             *
*******************************************************************************/
int board_load_int_element(const config_t *cfg_ptr, const config_setting_t * group, char *element)
{
  config_setting_t *setting = NULL;

  setting = config_setting_get_member(group, element);
  if(setting == NULL)
  {
    g_print("board_load_config(): Unable to find entry \"%s\" inside the \"matrix\" array !\n", element);
    config_destroy((struct config_t *) cfg_ptr);
    exit(-1);
  }
    
  return config_setting_get_int(setting);
}

/*******************************************************************************
* Connect to the interface                                                     *
*******************************************************************************/
int8_t board_connect(void)
{
  int flag_true = 1;
  struct termios termios_p;

  board_is_connected = FALSE;

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

/*******************************************************************************
* Disconnect from the interface                                                *
*******************************************************************************/
void board_disconnect(void)
{
  close(board_fd_ser);
  board_is_connected = FALSE;
}

/*******************************************************************************
* Check if the interface has recognized a new dart hit                         *
*******************************************************************************/
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

/*******************************************************************************
* Set the matrix type in the interface                                         *
*******************************************************************************/
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

/*******************************************************************************
* Get the firmware version from the interface                                  *
*******************************************************************************/
int8_t board_get_firmware_version(char *version, int version_size)
{
  int i;
  int recv_len, ret_val;
  unsigned char retry = 10;
  unsigned char cmd = 0x11;

  if(write(board_fd_ser, &cmd, 1) == -1)
    return BOARD_ERROR;
    
  recv_len = 0;

  while(retry)
  {
    ret_val = read(board_fd_ser, &board_recv_buffer[recv_len] , sizeof(board_recv_buffer) - recv_len);

    if(ret_val == -1)
    {
      if(errno != EAGAIN)
        return BOARD_ERROR;
    }
    else
    {
      if(ret_val > 0)
      {
        recv_len += ret_val;
        
        for(i=0; i<recv_len; i++)
        {
          // Search for strong end
          if(board_recv_buffer[i] == 0x0)
          {
            strncpy(version, (char *) board_recv_buffer, version_size);
            return BOARD_SUCCESS;
          }
        }
      }
    }

    usleep(100000);
    retry--;
  }

  return BOARD_ERROR;
}

/*******************************************************************************
* Check if a matrix configuration file exist. If not, create a new one.        *
*******************************************************************************/
void board_check_matrix_configuration_file(void)
{
  FILE *fd;
  gchar *filename;
  struct stat stat_buf;

  filename = g_build_filename(g_get_home_dir(), BOARD_FILENAME, NULL);
  if(g_stat(filename, &stat_buf) != 0)
  {
    fd = fopen(filename, "w");
    fputs("output_count    =   4;\n"                                           , fd);
    fputs("key_next_player = 254;\n"                                           , fd);
    fputs("key_bounce_out  = 255;\n"                                           , fd);
    fputs("\n"                                                                 , fd);
    fputs("matrix = (\n"                                                       , fd);
    fputs("  {segment =  1; single =   6; double  =   5; trible    =  11; },\n", fd);
    fputs("  {segment =  2; single =  18; double  =  17; trible    =  29; },\n", fd);
    fputs("  {segment =  3; single =  56; double  =  55; trible    =  58; },\n", fd);
    fputs("  {segment =  4; single =   2; double  =   1; trible    =  13; },\n", fd);
    fputs("  {segment =  5; single =  15; double  =  16; trible    =   9; },\n", fd);
    fputs("  {segment =  6; single =  24; double  =  23; trible    =  26; },\n", fd);
    fputs("  {segment =  7; single =  52; double  =  51; trible    =  60; },\n", fd);
    fputs("  {segment =  8; single =  47; double  =  48; trible    =  41; },\n", fd);
    fputs("  {segment =  9; single =  36; double  =  35; trible    =  44; },\n", fd);
    fputs("  {segment = 10; single =  22; double  =  21; trible    =  27; },\n", fd);
    fputs("  {segment = 11; single =  40; double  =  39; trible    =  42; },\n", fd);
    fputs("  {segment = 12; single =  34; double  =  33; trible    =  45; },\n", fd);
    fputs("  {segment = 13; single =  31; double  =  32; trible    =  25; },\n", fd);
    fputs("  {segment = 14; single =  38; double  =  37; trible    =  43; },\n", fd);
    fputs("  {segment = 15; single =  20; double  =  19; trible    =  28; },\n", fd);
    fputs("  {segment = 16; single =  50; double  =  49; trible    =  61; },\n", fd);
    fputs("  {segment = 17; single =  63; double  =  64; trible    =  57; },\n", fd);
    fputs("  {segment = 18; single =   4; double  =   3; trible    =  12; },\n", fd);
    fputs("  {segment = 19; single =  54; double  =  53; trible    =  59; },\n", fd);
    fputs("  {segment = 20; single =   8; double  =   7; trible    =  10; },\n", fd);
    fputs("  {segment = 25; single =  30; double  =  62; trible    =   0; }\n" , fd);
    fputs(");\n"                                                               , fd);
    fputs("\n"                                                                 , fd);
    
    fclose(fd);
  }
}
