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


#include "Polygon.hpp"
#include "Geometry.hpp"
#include "debugging.h"

using namespace jaephys;

//////////////////////////////////////////////////////////////////////////////
Polygon::Polygon ()
{
}

//////////////////////////////////////////////////////////////////////////////
Polygon &
Polygon::operator= ( const Polygon & other )
{
  vEdge_ = other.vEdge_;
  return *this;
}

//////////////////////////////////////////////////////////////////////////////
Polygon::Polygon ( const Polygon & other )
{
  vEdge_ = other.vEdge_;
}

//////////////////////////////////////////////////////////////////////////////
Edge &  
Polygon::edge( unsigned int index) 
{
  return vEdge_[index];
}

//////////////////////////////////////////////////////////////////////////////
unsigned int 
Polygon::numberOfEdges() const
{
  return vEdge_.size();
}

//////////////////////////////////////////////////////////////////////////////
const Coord3D &
Polygon::normal()
{
	updateNormal();
  GUARANTEE((normal_.isNull(0.0))==false,"Error: normal is null");
  return normal_;
}

///////////////////////////////////////////////////////////////////////////////
// Description:
//   Calcula si el point de entrada esta en el subespacio interior al plano que
//   contiene al polygon. Este espacio interior es el contrario al que apunta 
//   la normal.
// 
Logical
Polygon::isInside(const Coord3D & point)
{
	updateNormal();
	return isInsideThePlane(normal_, vEdge_[0].end1(), point);
}

void
Polygon::updateNormal()
{
	normal_.assignCrossProduct( vEdge_[0].direction(), vEdge_[1].direction());
}

///////////////////////////////////////////////////////////////////////////////
// Description:
//   returns if a given point contained in this polygon's plane belongs 
//   to the polygon
//   
bool
Polygon::contains ( const Coord3D & point, double error)
{
  Coord3D edgeNormal;

	updateNormal();
  
  for( unsigned int i= 0; i < vEdge_.size(); ++i)
  {
    edgeNormal.assignCrossProduct( vEdge_[i].direction(), normal_);

    if( isInsideThePlane( edgeNormal, vEdge_[i].end1(), point, error)
        == NO )
    {
      return false;
    }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
std::ostream & 
jaephys::operator<< ( std::ostream & os, const Polygon & polygon)
{
  os << "( " << polygon.edge(0) << std::endl;
  
  for
   ( unsigned int i = 1
   ; i < polygon.numberOfEdges()
   ; ++i
   )
  {
    os << "," << polygon.edge(i) << std::endl;
  }
  os << ")" << std::endl;

  return os;
}
