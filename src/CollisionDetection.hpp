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

#ifndef JAEPHYS_COLLISIONDETECTION_HPP
#define JAEPHYS_COLLISIONDETECTION_HPP

#include "Algorithm.hpp"
#include "auxiliary.hpp"

#include <vector>

//////////////////////////////////////////////////////////////////////////////
namespace jaephys
{
	class Entity;
  class Polygon;
  class Edge;
  class PolygonalShape;

  class CollisionDetection : virtual public Algorithm
  {
  public:
    CollisionDetection( Entity& entity1, 
                        Entity& entity2, 
                        RelativePosition relativePos1 );
    ~CollisionDetection();
    
    //CollisionDetection( const vector<Entity* > & lEntity);
    bool run();
    
  protected:
    bool detectSphericalPenetration();
    bool detectEdgePenetration( std::vector<Edge>    & vEdge, 
                                std::vector<Polygon> & vPolyg );

    PolygonalShape* shape1_;
    PolygonalShape* shape2_;
    RelativePosition relativePos1_;
    
  private:
    CollisionDetection();
    CollisionDetection(const CollisionDetection&);
    CollisionDetection&
      operator=(const CollisionDetection&);
      
  };
}

#endif // JAEPHYS_COLLISIONDETECTION_HPP
