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


#ifndef JAEPHYS_COORD3D_HPP
#define JAEPHYS_COORD3D_HPP

#include "const_enum_typedef.h"
#include <strstream>
#include <iostream>

namespace jaephys
{
  struct Coord3d 
  {
    double x;
    double y;
    double z;

    Coord3d();
    Coord3d( const Coord3d &);
    Coord3d( double x, double y, double z); 
		void set(double x, double y, double z);

    Coord3d & operator=( const Coord3d & p);
    bool operator==( const Coord3d & other) const;
    const Coord3d & operator/=(double factor);
    const Coord3d & operator*=(double factor);
    const Coord3d & operator+=( const Coord3d &p);
    const Coord3d & operator-=( const Coord3d &p);

    bool isNull(double error=ERROR_MARGIN) const;
        

    void normalize();
    void assignSubtraction( const Coord3d& addend, const Coord3d& subtrahend);
    void assignAddition( const Coord3d & addend1, const Coord3d & addend2);

    void assignCrossProduct ( const Coord3d & v1, const Coord3d & v2);
    void changeSign();
    void nullify(){ x = y = z = 0;}
    double dotProduct( const Coord3d & other) const;
    double module () const;

  };  
  ostream & operator<< ( ostream & os, const Coord3d & c);

  // for ordering Coord3ds in a container 
  struct compareCoord3d
  {
    bool operator()(const Coord3d & a, const Coord3d & b)
    {
      strstream s1, s2;
      s1 << a;
      s2 << b;
      return strcmp(s1.str(), s2.str());
    }
  };
  
} // jaephys

#endif // JAEPHYS_COORD3D_HPP
