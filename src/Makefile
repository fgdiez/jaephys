##############################################################################
#
# Jaephys - Just An Extendable PHYsics Simulator
#
# Copyright (C) 2001  Federico G. Diez <fedegdiez@hotmail.com>
#   
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
##############################################################################


CPPFLAGS=-W -Wall -pipe -g
CC=g++
SRC=.
OBJ=.obj
BIN=../bin
LIB=../lib
MKDIR=$(BIN) $(LIB) $(OBJ)

ALL: $(MKDIR) $(OBJ)/Algorithm.o $(OBJ)/ParseTree.o $(OBJ)/Edge.o $(OBJ)/Vertex.o $(OBJ)/Polygon.o $(OBJ)/PolygonalShape.o $(OBJ)/Coord3d.o $(OBJ)/Shape.o

$(OBJ):
	mkdir $(OBJ)
$(BIN):
	mkdir $(BIN)
$(LIB):
	mkdir $(LIB)

clean:
	rm -f $(OBJ)/*; rm -f $(BIN)/*; rm -f $(LIB)/*

$(OBJ)/%.o: %.cpp %.hpp
	$(CC) $(CPPFLAGS) -c $< -o $@
