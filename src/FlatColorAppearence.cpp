/******************************************************************************
 *
 * Jaephys - Just An Extendable PHYsics Simulator
 *
 * Copyright (C) 2001  Federico G. Diez <fedegdiez@hotmail.com>
 *   
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *****************************************************************************/

#include "FlatColorAppearence.hpp"
#include "Entity.hpp"
#include "Display3D.hpp"
#include "PolygonalShape.hpp"
#include "debugging.h"

using namespace jaephys;

FlatColorAppearence::FlatColorAppearence ( Entity & entity )
	: display3D_( Display3D::getCurrent())
{
  shape_= entity.findAttribute<Shape,PolygonalShape>( "Shape");
  GUARANTEE( shape_!=0,"PolygonalShape not found");

  color_ = entity.findAttribute<Coord3D>( "Color");
  GUARANTEE( color_!=0,"Color not found");
}

FlatColorAppearence::~FlatColorAppearence()
{
}

bool FlatColorAppearence::run()
{
  std::vector<Polygon>& vPol = shape_->vPol_;
  unsigned int finIt = vPol.size();
  const Coord3D & c = *color_;

  for(unsigned int i = 0; i < finIt; ++i)
  {
    display3D_.useColor((int)c.x, (int)c.y, (int)c.z);
    display3D_.drawFlatColorPolygon( vPol[i] );
  }
  return true;
}
