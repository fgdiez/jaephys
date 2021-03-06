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


#ifndef JAEPHYS_POLYGONALSHAPE_HPP
#define JAEPHYS_POLYGONALSHAPE_HPP

#include "Shape.hpp"
#include "Polygon.hpp"
#include "Edge.hpp"
#include "Vertex.hpp"

namespace jaephys
{
  enum EnumPolygonalShape
  { PS_CUBE
  , PS_PYRAMID
  };

//////////////////////////////////////////////////////////////////////////////
	class PolygonalShape : public virtual Shape
	{
	public:
	  // inherited
	  void translate( const Coord3D & vector);
	  void rotate( const Coord3D & w );


		// constructors, destructor
	  PolygonalShape();
	  virtual ~PolygonalShape();
    PolygonalShape(const PolygonalShape&);

    void createCube( double side );
    void createPyramid( double baseDiagonal, double height);

    double outerRadius() const{ return outerRadius_;}
    double innerRadius() const{ return innerRadius_;}

	  std::vector<Polygon> vPol_;
	  std::vector<Edge> vEdge_;
	  std::vector<Vertex> vPoint_;

	protected:
    double outerRadius_;
    double innerRadius_;

    void createAssociations();

	private:
   // not implemented
	 PolygonalShape & operator==( const PolygonalShape &);

	};
}

#endif // JAEPHYS_POLYGONALSHAPE_HPP
