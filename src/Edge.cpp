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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *****************************************************************************/


#include "Edge.hpp"
#include "Polygon.hpp"
#include "Geometry.hpp"

#include "debugging.h"

using namespace jaephys;

///////////////////////////////////////////////////////////////////////////////
Edge::Edge()
	: vPoint_(0)
  , vPol_(0)
{
}

///////////////////////////////////////////////////////////////////////////////
Edge &
Edge::operator= ( const Edge & other)
{
	vPoint_= other.vPoint_;
	iEnd1_= other.iEnd1_;
	iEnd2_= other.iEnd2_;

	vPol_= other.vPol_;
	iPol1_= other.iPol1_; 
	iPol2_= other.iPol2_;

  return *this;
}

///////////////////////////////////////////////////////////////////////////////
Edge::Edge( const Edge & other)
{
	vPoint_= other.vPoint_;
	iEnd1_= other.iEnd1_;
	iEnd2_= other.iEnd2_;

	vPol_= other.vPol_;
	iPol1_= other.iPol1_; 
	iPol2_= other.iPol2_;
}

///////////////////////////////////////////////////////////////////////////////
bool 
Edge::operator==(const Edge & other) const
{
	// we just compare the vertexes indexes
  if(iEnd1_ == other.iEnd1_)
  {
    return (iEnd2_ == other.iEnd2_);
  }
  if(iEnd1_ == other.iEnd2_)
  {
    return (iEnd2_ == other.iEnd1_);
  }
  return  false;
}


///////////////////////////////////////////////////////////////////////////////
const Coord3d & 
Edge::end1() const
{
  return ((*vPoint_)[iEnd1_]).point;
}

///////////////////////////////////////////////////////////////////////////////
const Coord3d & 
Edge::end2() const
{
  return ((*vPoint_)[iEnd2_]).point;
}
 
///////////////////////////////////////////////////////////////////////////////
const Coord3d &
Edge::direction()
{
	updateDirection();
  return direction_;
}

void
Edge::updateDirection()
{
	direction_.assignSubtraction(end2(),end1());
}

///////////////////////////////////////////////////////////////////////////////
// Description:
//   considering the edge as a fold in the surface defined by the two
//   polygons which joints it returns the normal vector of this surface
const Coord3d & 
Edge::normal()
{
  assert(vPol_ != 0);
  normal_.assignAddition( ((*vPol_)[iPol1_]).normal()
                        , ((*vPol_)[iPol1_]).normal()
                        );
  return normal_;
}

///////////////////////////////////////////////////////////////////////////////
// Description:
//   given a point contained in the line which defines the edge, it return
//   if the point belongs to the edge
bool 
Edge::contains( const Coord3d & point, double error)
{
	updateDirection();
  if( isPlaneInsidePoint(direction_, end2(), point, error) == NO ||
      isPlaneInsidePoint(direction_, end1(), point, error) == YES)
		return false;

	return true;
}

///////////////////////////////////////////////////////////////////////////////
ostream &
jaephys::operator<< ( ostream & os, const Edge & edge)
{
  os << "(" << edge.end1() << ", " << edge.end2() << ")";
  return os;
}
