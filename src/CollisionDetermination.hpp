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

#ifndef JAEPHYS_COLLISIONDETERMINATION_HPP
#define JAEPHYS_COLLISIONDETERMINATION_HPP

#include "Algorithm.hpp"
#include "Coord3D.hpp"

#include <vector>
#include <iostream>

namespace jaephys
{
	class Entity;
	class Edge;
	class PolygonalShape;
	class Polygon;

  enum EnumIntrusion{ VERTEX_POLYGON, EDGE_EDGE};

  ////////////////////////////////////////////////////////////////////////////
  struct Collision
  {
    Coord3D collisionPoint1; 
    Coord3D collisionPoint2; 
    Coord3D normal;
    double penetration;
    EnumIntrusion type;
    unsigned int id1; // collision element id
    unsigned int id2; 

    inline Collision & operator=( const Collision & other)
    {
      collisionPoint1 = other.collisionPoint1;
      collisionPoint2 = other.collisionPoint2;
      normal = other.normal;
      penetration = other.penetration;
      type= other.type;
      id1 = other.id1;
      id2 = other.id2;
      return *this;
    }
  };

  struct CollisionData{
    Coord3D v1; // linear velocity
    Coord3D w1; // angular velocity
    PolygonalShape* shape1;
    Coord3D v2; 
    Coord3D w2; 
    PolygonalShape* shape2;
    RelativePosition relPos;
  };

  /////////////////////////////////////////////////////////////////////////////
  // COMMITMENT
  // + Get collision point in each Entity
  // + Get collision plane (its normal)
  // + Undo the penetration
  //
  class CollisionDetermination: virtual public Algorithm
  {
  public:
    CollisionDetermination( Entity & e1, Entity & e2, RelativePosition relPos);
    virtual ~CollisionDetermination();

    bool run();

    Collision collision_;

  protected:

    bool determineCollision( CollisionData & cd, Collision & collision);

		void determineCollisions_VertexPolygon( CollisionData     & cd, 
																						std::vector<Collision> & vCollision );

		void determineCollisions_EdgeEdge( CollisionData     & cd,
																			 std::vector<Collision> & vCollision );

    bool analizeCandidates( const std::vector<Collision> & vCollisionVP, 
														const std::vector<Collision> & vCollisionEE,
														const RelativePosition    relPos,
														Collision               & collision );

    bool determineCollision_VertexPolygon( const Coord3D   & vertex,
																					 std::vector<Polygon> & vPol,
																					 const Coord3D & path,
																					 const Coord3D & v2, 
																					 const Coord3D & w2, 
																					 const Coord3D & centre2, 
																					 RelativePosition relPos,
																					 Collision & collision );

    bool determineCollision_EdgeEdge( Edge          & edge1,
																			std::vector<Edge>  & vEdge, 
																			const Coord3D & path, 
																			const Coord3D & v2,
																			const Coord3D & w2, 
																			const Coord3D & centre2,
																			Collision & collision );

    virtual bool repelIntrusion();
    
    bool checkCollision( CollisionData & cd, Collision & collision);

    bool checkCollision_VertexPolygon( const Coord3D & vertex, 
																		 Polygon       & polygon,
																		 const Coord3D & path,
																		 RelativePosition relPos );

		bool checkCollision_EdgeEdge( Edge & edge1,
																	Edge & edge2,
																	const Coord3D & path );
		
		bool checkVivacity( Collision & collision );

		bool checkVivacity( const Coord3D & vp1,
												const Coord3D & v2,
												const Coord3D & w2,
												const Coord3D & p2,
												const Coord3D & centre2,
												double distance );


    PolygonalShape *shape1_, *shape2_; 
    Coord3D *v1_, *v2_;    // linear velocity
    Coord3D *w1_, *w2_;    // angular velocity
    RelativePosition relPos1_, relPos2_;

  private:
    CollisionDetermination();
    CollisionDetermination( const CollisionDetermination & );
    CollisionDetermination & operator=( const CollisionDetermination & );
  };

  
  /////////////////////////////////////////////////////////////////////////////
  // Inline
  // 
  inline std::ostream & operator<<(std::ostream & os, const Collision & inter)
  {
    os << "collisionPoint1:" << inter.collisionPoint1 << std::endl;
    os << "collisionPoint2:" << inter.collisionPoint2 << std::endl;
    os << "normal:" << inter.normal << std::endl;
    os << "penetration:" << inter.penetration;
    return os;
  }

  /////////////////////////////////////////////////////////////////////////////
  inline std::ostream & operator<<(std::ostream & os, const std::vector<Collision> & v)
  {
    for(unsigned int i=0; i<v.size(); ++i)
    {
      os << v[i] << std::endl << std::endl;
    }
    return os;
  }

}

#endif // JAEPHYS_COLLISIONDETERMINATION_HPP
