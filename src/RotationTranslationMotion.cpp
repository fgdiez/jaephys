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

#include "RotationTranslationMotion.hpp"

using namespace jaephys;

RotationTranslationMotion::RotationTranslationMotion( Entity & entity )
{
  shape_ = entity.findAttribute<Shape>( "Shape" );
  GUARANTEE(shape_!=0,"Shape no encontrada" );

  v_ = entity.findAttribute<Coord3D>( "LinearVelocity" );
  GUARANTEE( v_ != 0, "Velocidad lineal no encontrada" );

  w_ = entity.findAttribute<Coord3D>( "AngularVelocity" );
  GUARANTEE( w_ != 0,"AngularVelocity not found" );
}

RotationTranslationMotion::~RotationTranslationMotion()
{
}

bool
RotationTranslationMotion::run()
{
  shape_->rotate( *w_ );
  shape_->translate(*v_);
  return true;
}

void RotationTranslationMotion::setFactor(const double factor)
{
  (*v_) *= factor;
  (*w_) *= factor;
}
