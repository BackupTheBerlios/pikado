################################################################################
#  Pikado - Interface electronic and GUI for a PC based e-dart machine         #
#  Copyright (C) 2006 - 2010 Matthias Klein <matthias.klein@linux.com>         #
#                                                                              #
#  This file is part of Pikado.                                                #
#                                                                              #
#  Pikado is free software: you can redistribute it and/or modify              #
#  it under the terms of the GNU General Public License as published by        #
#  the Free Software Foundation, either version 3 of the License, or           #
#  (at your option) any later version.                                         #
#                                                                              #
#  Pikado is distributed in the hope that it will be useful,                   #
#  but WITHOUT ANY WARRANTY; without even the implied warranty of              #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               #
#  GNU General Public License for more details.                                #
#                                                                              #
#  You should have received a copy of the GNU General Public License           #
#  along with Pikado.  If not, see <http://www.gnu.org/licenses/>.             #
################################################################################
CC      = gcc
CFLAGS  = -Wall -g `pkg-config --cflags gtk+-2.0` -DG_DISABLE_DEPRECATED
LDFLAGS = -Wall -g `pkg-config --libs gtk+-2.0`
###############################################################################
DEPENDFILE = .depend

%.o: %.c
	$(CC) $(CFLAGS) -c $<
###############################################################################
.PHONY: all depend clean

all: depend pikado

pikado: pikado.o config.o board.o screen_main.o screen_single.o
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -f pikado *.o $(DEPENDFILE)

depend:
	$(CC) -MM *.c > $(DEPENDFILE)

-include $(DEPENDFILE)

