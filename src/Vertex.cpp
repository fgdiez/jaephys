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


#include "Vertex.hpp"
#include "Polygon.hpp"
using namespace jaephys;

Vertex::Vertex()
	:vPol_(0)
{
}

Vertex::Vertex(const Vertex & other)
{
	point= other.point;
	vPol_= other.vPol_;
	viPol_= other.viPol_;
}

Vertex & 
Vertex::operator=( const Vertex & other)
{
  point = other.point;
	vPol_ = other.vPol_;
	viPol_= other.viPol_;

  return *this;
}

///////////////////////////////////////////////////////////////////////////////
// Description:
//   stores internaly a link to the polygons which this vertex is part of
//   
void Vertex::isPartOf( const vector<int> & viPol)
{
	viPol_= viPol;
}

///////////////////////////////////////////////////////////////////////////////
const Coord3d & 
Vertex::normal()
{
	updateNormal();
	return normal_;
}

///////////////////////////////////////////////////////////////////////////////
void
Vertex::updateNormal()
{
  assert(vPol_!= 0);
  normal_.nullify();
  for
   ( vector<int>::const_iterator it= viPol_.begin()
   ; it< viPol_.end()
   ; ++it)
  {
    normal_ += ((*vPol_)[*it]).normal();
  }

	normal_.normalize();
}
