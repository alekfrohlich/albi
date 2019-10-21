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

# Directories.
export INCDIR   = $(CURDIR)/include
export SRCDIR   = $(CURDIR)/src
export GRODIR   = $(CURDIR)/gro

# Toolchain configuration.
export CFLAGS   = -g -I $(INCDIR)

.PHONY: albi

# Builds the parser.
albi:
	cd $(SRCDIR) && $(MAKE) all

# Cleans compilation files.
clean:
	cd $(SRCDIR) && $(MAKE) clean