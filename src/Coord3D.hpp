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

#include "auxiliary.hpp"

#include <sstream>
#include <iostream>
#include <string>

namespace jaephys
{
  struct Coord3D 
  {
    double x;
    double y;
    double z;

    Coord3D();
    Coord3D( const Coord3D &);
		Coord3D( const std::string & str);
    Coord3D( double x, double y, double z); 
		void set(double x, double y, double z);

    Coord3D & operator=( const Coord3D & p);
    bool operator==( const Coord3D & other) const;
    const Coord3D & operator/=(double factor);
    const Coord3D & operator*=(double factor);
    const Coord3D & operator+=( const Coord3D &p);
    const Coord3D & operator-=( const Coord3D &p);

    bool isNull(double error=ERROR_MARGIN) const;
        

    void normalize();
    void assignSubtraction( const Coord3D& addend, const Coord3D& subtrahend);
    void assignAddition( const Coord3D & addend1, const Coord3D & addend2);

    void assignCrossProduct ( const Coord3D & v1, const Coord3D & v2);
    void changeSign();
    void nullify(){ x = y = z = 0;}
    double dotProduct( const Coord3D & other) const;
    double module () const;

  };  
  std::ostream & operator<< ( std::ostream & os, const Coord3D & c);

  // for ordering Coord3Ds in a container 
  struct compareCoord3D
  {
    bool operator()(const Coord3D & a, const Coord3D & b)
    {
      std::stringstream s1, s2;
      s1 << a;
      s2 << b;
      return s1.str() == s2.str();
    }
  };
  
} // jaephys

#endif // JAEPHYS_COORD3D_HPP
