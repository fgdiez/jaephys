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

#ifndef JAEPHYS_PARTICLECOLLISION_HPP
#define JAEPHYS_PARTICLECOLLISION_HPP

#include "Algorithm.hpp"

namespace jaephys
{
	class Entity;
	class Coord3D;

  class ParticleCollision: public virtual Algorithm
  {
  public:
    ParticleCollision( Entity & e1, Entity & e2, const Coord3D & normal);

    ~ParticleCollision();
    
    bool run();
    
  protected:
    void resolveFrontalCollision( const double v1, const double v2, 
																	double & v1_res, double & v2_res );

    Coord3D* v1_; // velocity 1
    Coord3D* v2_; // velocity 2
    const Coord3D & n_;
    const double * m1_; // mass 1
    const double * m2_; // mass 2
    
  private:
    ParticleCollision();
    ParticleCollision(const ParticleCollision &);
    ParticleCollision& operator=(const ParticleCollision&);
  };
}

#endif // JAEPHYS_PARTICLECOLLISION_HPP
