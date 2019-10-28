#   
# Copyright (C) 2019 Alek Frohlich <alek.frohlich@gmail.com> 
# & Gustavo Biage <gustavo.c.biage@gmail.com>.
# 
#  This file is a part of Albi.
# 
# Albi is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# 
# Albi is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. 
#

# C source files.
C_SRC = $(wildcard *.c)

# Additional compiler flags.
CFLAGS += -lm

ifndef PERMITED
$(error Do not execute this makefile directly, \
	please run make from the root directory!)
endif

all: albi

albi.tab.c albi.tab.h: albi.y
	bison -d $<

albi.yy.c: albi.l albi.tab.h
	flex -o $@ $<

albi: $(C_SRC) albi.yy.c albi.tab.c
	gcc $^ $(CFLAGS) -o translator

.PHONY: clean

clean:
	@rm -f albi.tab.* albi.yy.c translator