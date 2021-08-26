#*************************************************************************
#** Copyright (C) 2013 Jan Pedersen <jp@jp-embedded.com>
#** 
#** This program is free software: you can redistribute it and/or modify
#** it under the terms of the GNU General Public License as published by
#** the Free Software Foundation, either version 3 of the License, or
#** (at your option) any later version.
#** 
#** This program is distributed in the hope that it will be useful,
#** but WITHOUT ANY WARRANTY; without even the implied warranty of
#** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#** GNU General Public License for more details.
#** 
#** You should have received a copy of the GNU General Public License
#** along with this program.  If not, see <http://www.gnu.org/licenses/>.
#*************************************************************************

# URL := "$HeadURL: http://scxmlcc.googlecode.com/svn/tags/0.2.1/src/makefile $"
# URL_TRUNK := $(findstring trunk,$(URL))
# URL_TAG := $(firstword $(subst /, ,$(word 3,$(subst tags, ,$(URL)))))
# VER := $(or $(URL_TRUNK), $(URL_TAG), unknown)

OBJS := main.o cpp_output.o scxml_parser.o version.o
CPPFLAGS := -Wall -O2 -MD -MP -std=c++0x 
# -DVER="$(VER)"

all: escxmlcc

escxmlcc: $(OBJS)
	$(CXX) -o $@ $^ -lboost_program_options -lboost_system -lboost_filesystem

clean:
	rm -f $(OBJS) $(OBJS:.o=.d) escxmlcc


-include $(OBJS:.o=.d)

