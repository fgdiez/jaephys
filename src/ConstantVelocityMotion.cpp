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

#include "ConstantVelocityMotion.hpp"

#include "Attribute.hpp"
#include "Geometry.hpp"
#include "debugging.h"

using namespace jaephys;

///////////////////////////////////////////////////////////////////////////////
ConstantVelocityMotion::ConstantVelocityMotion( Entity & entity )
{
  TRACEC(JAEPHYS_ANIM,1,"enter");

  shape_ = entity.findAttribute<Shape>( "Shape" );
  GUARANTEE( shape_!=0,"Shape no encontrada" );

  velocity_= entity.findAttribute<Coord3D>( "LinearVelocity" );
  GUARANTEE( velocity_!=0, "LinearVelocity not found" );
}

ConstantVelocityMotion::~ConstantVelocityMotion()
{
}

///////////////////////////////////////////////////////////////////////////////
bool
ConstantVelocityMotion::run()
{
  TRACEC(JAEPHYS_ANIM,2,"enter");
  shape_->translate(*velocity_);
  return true;
}

void ConstantVelocityMotion::setFactor(const double factor)
{
  TRACEC(JAEPHYS_ANIM,2,"enter");
  GUARANTEE(factor > 0,"factor="<< factor << " is negative");

  (*velocity_) *= factor;
  TRACEC(JAEPHYS_ANIM,2,"velocity="<<*velocity_);
}

