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

#include "WireAppearence.hpp"
#include "Display3D.hpp"
#include "Entity.hpp"
#include "PolygonalShape.hpp"
#include "debugging.h"

using namespace jaephys;

WireAppearence::WireAppearence ( Entity & entity )
	: display3D_( Display3D::getCurrent() )
{
  shape_= dynamic_cast< PolygonalShape* >(
      entity.findAttribute< Shape >( "Shape" ) );

  GUARANTEE( shape_ != 0, "Shape poligonal no encontrada" );

  color_ = entity.findAttribute<Coord3D>( "Color" );
  GUARANTEE( color_ != 0 , "Color not found" );
}

WireAppearence::~WireAppearence()
{
}

bool WireAppearence::run()
{
  std::vector<Polygon>& vPol = shape_->vPol_;
  unsigned int size = vPol.size();
  display3D_.useColor( (int)color_->x, (int)color_->y, (int)color_->z);

  for( unsigned int i = 0; i < size; ++i )
  {
    display3D_.drawWiredPolygon( vPol[i] );
  }

  return true;
}
