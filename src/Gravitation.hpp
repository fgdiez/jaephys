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

#ifndef JAEPHYS_GRAVITATION_HPP
#define JAEPHYS_GRAVITATION_HPP

#include "Algorithm.hpp"

//////////////////////////////////////////////////////////////////////////////
namespace jaephys
{
	class Entity;
	class Coord3D;

  class Gravitation : virtual public Algorithm
  {
  public:
    Gravitation( Entity& entity1, Entity& entity2 );
    ~Gravitation();
    
    bool run();
    
  protected:
    const Coord3D* c1_;
    const Coord3D* c2_;
    Coord3D* a1_;
    Coord3D* a2_;
    double* m1_;
    double* m2_;

  private:
    Gravitation();
    Gravitation(const Gravitation&);
    Gravitation& operator=(const Gravitation&);
  };
}

#endif //JAEPHYS_GRAVITATION_HPP
