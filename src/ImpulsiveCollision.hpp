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

#ifndef JAEPHYS_IMPULSIVECOLLISION_HPP
#define JAEPHYS_IMPULSIVECOLLISION_HPP

#include "Algorithm.hpp"

namespace jaephys
{
	class Entity;
	class Collision;
	class Coord3D;
	class PolygonalShape;

  class ImpulsiveCollision: public virtual Algorithm
  {
  public:
    ImpulsiveCollision( Entity          & entity1, 
												Entity          & entity2, 
												const Collision & collision );
    ~ImpulsiveCollision();
    
    bool run();
    
  protected:

    const Collision & collision_;
    Coord3D *v1_; // Linear Velocity
    Coord3D *v2_; 
    Coord3D *w1_; // Angular Velocity 
    Coord3D *w2_;
    double *m1_;  // Mass
    double *m2_; 

    Coord3D *L1_; // Angular Momentum
    Coord3D *L2_; 
    PolygonalShape* shape1_;
    PolygonalShape* shape2_;

  private:
    ImpulsiveCollision();
    ImpulsiveCollision(const ImpulsiveCollision &);
    ImpulsiveCollision& operator=(const ImpulsiveCollision&);
  };
}

#endif // JAEPHYS_IMPULSIVECOLLISION_HPP
