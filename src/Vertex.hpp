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


#ifndef JAEPHYS_VERTEX_HPP
#define JAEPHYS_VERTEX_HPP

#include "Coord3D.hpp"

#include <vector>

namespace jaephys
{
	// predefinition
  class Polygon;

  class Vertex
  {
   public:
    Vertex();
    Vertex(const Vertex &);
		Vertex(std::vector<Polygon>* vPol) { vPol_= vPol;}
    Vertex & operator=( const Vertex & other);
    
		void polygons(std::vector<Polygon>* vPol) 
      { vPol_= vPol;}
    void isPartOf( const std::vector<int> & viPol) 
      { viPol_= viPol;}

    operator Coord3D() const{ return point;}
    const Coord3D & normal();

		// attributes
    Coord3D point;

   private:
		void updateNormal();

		// attributes
		//
		std::vector<Polygon>* vPol_; // pointer to the container of polygons
		std::vector<int> viPol_;     // indexes of polygons that contains this vertex
		Coord3D normal_;        // polygons surface normal in this point

  };
}
#endif // JAEPHYS_VERTEX_HPP
