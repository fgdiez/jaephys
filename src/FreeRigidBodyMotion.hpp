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

#ifndef JAEPHYS_FREERIGIDBODYMOTION_HPP
#define JAEPHYS_FREERIGIDBODYMOTION_HPP

#include "PolygonalShape.hpp"
#include "Algorithm.hpp"
#include "Entity.hpp"

namespace jaephys
{
	// Free Rigid-Body Motion approximated as a particle system
  class FreeRigidBodyMotion : virtual public AnimationAlgorithm
  {
  public:
    FreeRigidBodyMotion( Entity & entity );
    ~FreeRigidBodyMotion();

    bool run();
    void setFactor(const double factor);
    
  protected:
    PolygonalShape* shape_;
		Coord3D* v_;
    Coord3D* L_; // Angular Momentum
    Coord3D* w_;
    double* mass_;

  private:
    FreeRigidBodyMotion();
    FreeRigidBodyMotion( const FreeRigidBodyMotion& );
    FreeRigidBodyMotion & operator=( const FreeRigidBodyMotion &);  
  };
}

#endif //JAEPHYS_FREERIGIDBODYMOTION_HPP
