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


#ifndef JAEPHYS_POLYGON_H
#define JAEPHYS_POLYGON_H

#include "Coord3D.hpp"
#include "Edge.hpp"

#include <vector>

namespace jaephys
{
  class Polygon
  {
  public:
    Polygon();
    Polygon( const Polygon &);
    Polygon & operator= (const Polygon &);
    
		std::vector<Edge>& edges() { return vEdge_;}

    const Edge & edge( unsigned int index) const {return vEdge_[index];}
    Edge & edge( unsigned int index);

    unsigned int numberOfEdges() const;
    const Coord3D & normal();

    Logical isInside(const Coord3D & point);
    bool contains( const Coord3D & point, double error=ERROR_MARGIN);
    
  private:
		void updateNormal();

    std::vector<Edge> vEdge_;
    Coord3D normal_;
  };

  std::ostream & operator<< ( std::ostream & os, const Polygon & polygon);

} // jaephys
#endif // JAEPHYS_POLYGON_H
