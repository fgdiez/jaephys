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


CPPFLAGS=-W -Wall
CC=gcc
SRC=.
OBJ=.obj
BIN=../bin
MKDIR=$(BIN) $(OBJ)

LIBD= -lglut -lGL -lGLU -L /usr/X11R6/lib 

OBJS=$(OBJ)/Algorithm.o $(OBJ)/ParseTree.o $(OBJ)/Edge.o $(OBJ)/Vertex.o $(OBJ)/Polygon.o $(OBJ)/PolygonalShape.o $(OBJ)/Coord3D.o $(OBJ)/Shape.o $(OBJ)/Geometry.o $(OBJ)/Entity.o $(OBJ)/auxiliary.o $(OBJ)/ConstantVelocityMotion.o $(OBJ)/RotationTranslationMotion.o $(OBJ)/AcceleratedMotion.o $(OBJ)/FreeRigidBodyMotion.o $(OBJ)/Display3D.o $(OBJ)/FlatColorAppearence.o $(OBJ)/WireAppearence.o $(OBJ)/PunkWireAppearence.o $(OBJ)/FEntity.o $(OBJ)/Interaction.o $(OBJ)/Display3D_gl.o $(OBJ)/FDisplay.o $(OBJ)/ParticleCollision.o $(OBJ)/Keyboard.o $(OBJ)/Parser.o $(OBJ)/Demiurge.o $(OBJ)/CollisionDetection.o $(OBJ)/Gravitation.o $(OBJ)/FInteraction.o $(OBJ)/CollisionDetermination.o $(OBJ)/jae.o $(OBJ)/ImpulsiveCollision.o

all: $(BIN)/jae

$(BIN)/jae: $(MKDIR) $(OBJS)
	$(CC) $(OBJS) $(LIBD) -o $(BIN)/jae

$(OBJ):
	mkdir $(OBJ)
$(BIN):
	mkdir $(BIN)


clean:
	rm -f $(OBJ)/*; rm -f $(BIN)/jae

$(OBJ)/%.o: %.cpp %.hpp
	$(CC) $(CPPFLAGS) -c $< -o $@

$(OBJ)/%.o: %.c %.h
	$(CC) $(CPPFLAGS) -c $< -o $@

$(OBJ)/%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@
