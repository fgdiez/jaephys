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

#include "AcceleratedMotion.hpp"

using namespace jaephys;

///////////////////////////////////////////////////////////////////////////////
AcceleratedMotion::AcceleratedMotion( Entity & entity )
  : factor_(1),
		w_(0),
		rotate_(false)
{
  a_= entity.findAttribute<Coord3D>( "Acceleration" );
  GUARANTEE( a_!=0, "Acceleration not found" );
  v_= entity.findAttribute<Coord3D>( "LinearVelocity" );
  GUARANTEE(v_!=0,"LinearVelocity not found" );
  shape_= entity.findAttribute<Shape>( "Shape" );
  GUARANTEE(shape_!=0,"Shape not found" );
  w_= entity.findAttribute<Coord3D>("AngularVelocity");
	if(w_ != 0)
		 rotate_= true;
}

///////////////////////////////////////////////////////////////////////////////
AcceleratedMotion::~AcceleratedMotion()
{
}

///////////////////////////////////////////////////////////////////////////////
bool AcceleratedMotion::run()
{
  (*a_) *= factor_;
  TRACEC( JAEPHYS_GRAV, 1, "Aceleracion=" << *a_ );

  (*v_) += (*a_);
  TRACEC( JAEPHYS_GRAV, 1, "Velocidad=" << *v_ );

  shape_->translate(*v_);
  TRACEC( JAEPHYS_GRAV, 2, "centre=" << shape_->centre() );

  a_->nullify();

	if(rotate_ == true){
		GUARANTEE(w_ != 0,"Null pointer to angular velocity");
		shape_->rotate(*w_);
	}
  return true;
}

///////////////////////////////////////////////////////////////////////////////
void AcceleratedMotion::setFactor(double factor)
{
  (*v_) *= factor;
  factor_ = factor;
  if(w_ != 0)
		*w_ *= factor;
}
