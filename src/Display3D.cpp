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

#include "Display3D.hpp"
#include "Display3D_gl.hpp"
#include "Geometry.hpp"
#include "debugging.h"

#include <math.h>

using namespace jaephys;

Display3D* Display3D::current_ = 0;

Display3D::Display3D()
	: width_(0),
		height_(0)
{
  viewZdir_ = Coord3D(0,0,1);
  viewYdir_ = Coord3D(0,1,0);
  viewXdir_.assignCrossProduct( viewYdir_, viewZdir_ );
  flushViewMatrix();
}

Display3D::~Display3D(){}

Display3D & Display3D::create( EnumDisplay3D type, int width, int height )
{
	
  switch( type ){
	case DISPLAY3D_GL:
		current_= new Display3D_gl( width, height);
		break;
	default:
		GUARANTEE(0, "Not a valid Display3D type");
	}
	return *(current_);
}

Display3D & Display3D::getCurrent()
{
	GUARANTEE(current_, "Attempting to access null Display3D");
	return *current_;
}

const Coord3D & Display3D::getCentre() const
{
  return centre_;
}

void Display3D::flushViewMatrix()
{
  mView_[0][0]=viewXdir_.x;
  mView_[0][1]=viewXdir_.y;
  mView_[0][2]=viewXdir_.z;

  mView_[1][0]=viewYdir_.x;
  mView_[1][1]=viewYdir_.y;
  mView_[1][2]=viewYdir_.z;

  mView_[2][0]=viewZdir_.x;
  mView_[2][1]=viewZdir_.y;
  mView_[2][2]=viewZdir_.z;
  TRACEC(JAEPHYS_DISP,2,"mView=\n" << mView_);
}

//////////////////////////////////////////////////////////////////////////////
void Display3D::applyView(const Coord3D & p, Coord3D & pTransformed) const
{
  static Coord3D aux;
  aux.assignSubtraction( p, viewPosition_ );
  multiply( mView_, aux, pTransformed );
}

void Display3D::translateView(EnumAxis axis, double increment)
{
  switch(axis){
  case AXIS_X:
    viewPosition_.x += viewXdir_.x*increment;
    viewPosition_.y += viewXdir_.y*increment;
    viewPosition_.z += viewXdir_.z*increment;
    break;
  case AXIS_Y:
    viewPosition_.x += viewYdir_.x*increment;
    viewPosition_.y += viewYdir_.y*increment;
    viewPosition_.z += viewYdir_.z*increment;
    break;
  case AXIS_Z:
    viewPosition_.x += viewZdir_.x*increment;
    viewPosition_.y += viewZdir_.y*increment;
    viewPosition_.z += viewZdir_.z*increment;
    break;
  default:
    GUARANTEE(false,"Axis not valid");
  }
}

void Display3D::rotateView(EnumAxis axis, double angle)
{
  switch(axis){
  case AXIS_X:
    rotate2Axes(angle, viewYdir_, viewZdir_);
    break;
  case AXIS_Y:
    rotate2Axes(angle, viewZdir_, viewXdir_);
    break;
  case AXIS_Z:
    rotate2Axes(angle, viewXdir_, viewYdir_);
    break;
  default:
    GUARANTEE(0, "Incorrect axis");
  }
}

///////////////////////////////////////////////////////////////////////////////
// COMMITMENT: rotates two given vectors around the axis perpendicular to
//   the plane that they define together
//
void Display3D::rotate2Axes(double angle, Coord3D& v1, Coord3D& v2)
{
  double v1_x= v1.x;
  double v1_y= v1.y;
  double v1_z= v1.z;
  double tangent= tan(angle);
  v1.x += v2.x*tangent;
  v1.y += v2.y*tangent;
  v1.z += v2.z*tangent;

  v2.x -= v1_x*tangent;
  v2.y -= v1_y*tangent;
  v2.z -= v1_z*tangent;

  double module_inv= static_cast<double>(1)/sqrt(1 + tangent*tangent);
  v1 *= module_inv;
  v2 *= module_inv;
}
