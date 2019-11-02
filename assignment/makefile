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

.PHONY: albi test

# Directories.
export INCDIR  	= $(CURDIR)/include
export SRCDIR  	= $(CURDIR)/src
export GRODIR  	= $(CURDIR)/gro
export FAULTDIR = $(GRODIR)/faulty

# Gro translator.
export TRANSLATOR = $(SRCDIR)/translator

# Toolchain configuration.
export CFLAGS   = -g -I $(INCDIR)

# Deny make calls to sub-makefikes.
export PERMITED = 1

# Albi input's file name.
ALBI_SRC = $(patsubst $(GRODIR)/%, %, $(gro))

# Test macro
DOTEST = (echo "\nTesting$(ALBI_SRC)...\n") \
		  && ($(TRANSLATOR) $(gro) $(ALBI_SRC)) \
		  || (echo "TEST $(ALBI_SRC) FAILED!"; exit 1);

# Builds the parser.
albi:
	cd $(SRCDIR) && $(MAKE) all

test: albi
	@$(foreach gro, $(wildcard $(GRODIR)/*.gro), $(DOTEST))
	@echo "SUCCESS!"

fault: albi
	@echo "\nA demonstration of Albi's syntax errors...\n"
	@$(TRANSLATOR) $(FAULTDIR)/invalid_syntax.gro invalid_syntax.gro

# Cleans compilation files.
clean:
	cd $(SRCDIR) && $(MAKE) clean
	@rm -f parsed.intermediate
	@rm -f output.sbml
