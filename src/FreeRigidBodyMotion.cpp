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

#include "FreeRigidBodyMotion.hpp"

#include "Geometry.hpp"
#include "debugging.h"

using namespace jaephys;

FreeRigidBodyMotion::FreeRigidBodyMotion( Entity & entity )
{
  shape_ = dynamic_cast< PolygonalShape* >(
      entity.template findAttribute< Shape >( "Shape"));
  
  v_ = entity.template findAttribute< Coord3D >( "LinearVelocity");
  GUARANTEE(v_!=0,"LinearVelocity not found");

  L_ = entity.template findAttribute< Coord3D >( "AngularMomentum");
  GUARANTEE(L_!=0,"AngularMomentum not found");

  w_ = entity.template findAttribute< Coord3D >( "AngularVelocity");
  GUARANTEE(w_!=0,"AngularVelocity not found");

  mass_ = entity.template findAttribute<double>( "Mass");
  GUARANTEE(mass_!=0,"Mass not found");

  getAngularMomentum(*shape_, *w_, *mass_, *L_);
  TRACEC(JAEPHYS_ANIM,1,"id="<< shape_->id());
  TRACEC(JAEPHYS_ANIM,1,"L="<< (*L_));
}

FreeRigidBodyMotion::~FreeRigidBodyMotion()
{
}

bool
FreeRigidBodyMotion::run()
{
  Matrix3x3 I;
  getInertialTensor( I, shape_->vPoint_, shape_->centre(), *mass_);
  TRACEC(JAEPHYS_ANIM,1, "I:\n" << I);

  resolveSystem( I, *L_, *w_);

  TRACEC(JAEPHYS_ANIM,1, "w=" << *w_);

  shape_->rotate(*w_);
  
  shape_->translate(*v_);
  return true;
}

void FreeRigidBodyMotion::setFactor( const double factor )
{
  (*v_) *= factor;
  (*w_) *= factor;

  getAngularMomentum( *shape_, *w_, *mass_, *L_ );
}
