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


#ifndef JAEPHYS_EDGE_HPP
#define JAEPHYS_EDGE_HPP

#include "Coord3d.hpp"
#include "Vertex.hpp"

#include <vector>

namespace jaephys
{
  class Polygon;
  class Edge
  {
  public:

    Edge();
    Edge( const Edge & other);
    Edge & operator=( const Edge &); 
		Edge(vector<Vertex>* vPoint) { vPoint_= vPoint;}

		void vertexes(vector<Vertex>* vPoint) { vPoint_= vPoint;}
		void polygons(vector<Polygon>* vPol) { vPol_= vPol;}
		void ends(int iEnd1, int iEnd2) { iEnd1_= iEnd1; iEnd2_= iEnd2;}
		void isPartOf(int iPol1, int iPol2) { iPol1_= iPol1; iPol2_= iPol2;}

    const Coord3d & end1() const;
    const Coord3d & end2() const;
    int endIndex1() const { return iEnd1_;}
    int endIndex2() const { return iEnd2_;}

    const Coord3d & direction();
    const Coord3d & normal();
    
    bool operator==(const Edge &) const;
    bool contains( const Coord3d & point, double error=ERROR_MARGIN);

  private:
		void Edge::updateDirection();
		
		vector<Vertex>* vPoint_;  //pointer to the container of vertex
    
    int iEnd1_;               // indexes of ends relative to vPoint_
    int iEnd2_;
    
    Coord3d direction_;       // edge direction (from end1 to end2)

		vector<Polygon>* vPol_;   // pointer to the container of polygons
		
		int iPol1_;               // indexes of polygons joined by this edge
		int iPol2_;
		
    Coord3d normal_;          // normal vector at the joint
  };

  ostream & operator<< ( ostream & os, const Edge & edge);

} // jaephys
#endif // JAEPHYS_EDGE_HPP
