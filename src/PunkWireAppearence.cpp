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

#include "PunkWireAppearence.hpp"
#include "PolygonalShape.hpp"
#include "Entity.hpp"
#include "Display3D.hpp"

using namespace jaephys;

PunkWireAppearence::PunkWireAppearence( Entity & entity )
 : display3D_( Display3D::getCurrent() )
{
  shape_= dynamic_cast< PolygonalShape* >(
      entity.findAttribute< Shape >( "Shape" ) );

  GUARANTEE(shape_!=0,"PolygonalShape not found");

  color_ = entity.findAttribute<Coord3D>( "Color" );
  GUARANTEE(color_!=0,"Color not found");
}

PunkWireAppearence::~PunkWireAppearence()
{
}

bool PunkWireAppearence::run()
{
  display3D_.useColor((int)color_->x, (int)color_->y, (int)color_->z);

  std::vector<Polygon>& vPol= shape_->vPol_;
  unsigned int size = vPol.size();

  static Coord3D p1;
  static Coord3D p2;

	// draw a line from the centre of each polygon in the direction of its normal
  for(unsigned int i = 0; i < size; ++i)
  {
    Polygon& polyg= vPol[i];
    display3D_.drawFlatColorPolygon( polyg );
   
    p1.nullify();
    for( unsigned int j=0; j<polyg.numberOfEdges(); ++j)
    {
      p1 += (polyg.edge(j)).end1();
    }
    p1 /= polyg.numberOfEdges();
    p2.assignAddition( p1, polyg.normal() );
    display3D_.drawLine( p1, p2 );
  }

	// draw a line from each vertex
  std::vector<Vertex>& vPoint= shape_->vPoint_;
  for(unsigned int i=0;i<vPoint.size(); ++i)
  {
    Vertex& v = vPoint[i];
		Coord3D n(v.normal());
    p1.assignAddition( v.point, n *= 100);
    display3D_.drawLine( v.point, p1);
  }

	// draw a line from the middle of each edge
  std::vector<Edge>& vEdge= shape_->vEdge_;
  for(unsigned int i=0; i < vEdge.size(); ++i)
  {
    Edge & edge= vEdge[i];
    p1.assignAddition( edge.end1(), edge.end2());
    p1 /= 2;
    p2.assignAddition( p1, edge.normal() );
    display3D_.drawLine( p1, p2 );
  }

  return true;
}
