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


#include "Coord3d.hpp"
#include "Geometry.hpp"
#include "debugging.h"

#include <math.h>

using namespace jaephys;

//////////////////////////////////////////////////////////////////////////////
Coord3d::Coord3d()
 : x(0)
 , y(0)
 , z(0)
{}

//////////////////////////////////////////////////////////////////////////////
Coord3d::Coord3d( const Coord3d & p)
 : x(p.x)
 , y(p.y)
 , z(p.z)
{}

//////////////////////////////////////////////////////////////////////////////
Coord3d::Coord3d( double x0, double y0, double z0)
 : x(x0)
 , y(y0)
 , z(z0)
{} 

//////////////////////////////////////////////////////////////////////////////
Coord3d & 
Coord3d::operator=( const Coord3d &p)
{
  x = p.x;
  y = p.y;
  z = p.z;
  return *this;
}

//////////////////////////////////////////////////////////////////////////////
const Coord3d & 
Coord3d::operator+=( const Coord3d & p)
{
  x += p.x;
  y += p.y;
  z += p.z;
  return *this;
}

//////////////////////////////////////////////////////////////////////////////
const Coord3d & 
Coord3d::operator-=( const Coord3d & p)
{
  x -= p.x;
  y -= p.y;
  z -= p.z;
  return *this;
}

//////////////////////////////////////////////////////////////////////////////
const Coord3d & 
Coord3d::operator*=( double factor)
{
  x *= factor;
  y *= factor;
  z *= factor;
  return *this;
}

//////////////////////////////////////////////////////////////////////////////
const Coord3d & 
Coord3d::operator/=( double factor)
{
  double div= static_cast<double>(1)/factor;
  x *= div;
  y *= div;
  z *= div;
  return *this;
}

//////////////////////////////////////////////////////////////////////////////
double 
Coord3d::module() const
{
  return sqrt(x*x + y*y + z*z);
}

//////////////////////////////////////////////////////////////////////////////
void 
Coord3d::assignSubtraction(
  const Coord3d & addend,
  const Coord3d & subtrahend
)
{
  x = addend.x - subtrahend.x;
  y = addend.y - subtrahend.y;
  z = addend.z - subtrahend.z;
}

//////////////////////////////////////////////////////////////////////////////
void 
Coord3d::assignAddition (
  const Coord3d & addend1,
  const Coord3d & addend2
)
{
  x = addend1.x + addend2.x;
  y = addend1.y + addend2.y;
  z = addend1.z + addend2.z;
}

//////////////////////////////////////////////////////////////////////////////
void 
Coord3d::assignCrossProduct
  ( const Coord3d & v1
  , const Coord3d & v2
  )
{
  x = v1.y*v2.z - v1.z*v2.y;
  y = v1.z*v2.x - v1.x*v2.z;
  z = v1.x*v2.y - v1.y*v2.x;
}

//////////////////////////////////////////////////////////////////////////////
void 
Coord3d::changeSign()
{
  x = -x;
  y = -y;
  z = -z;
}

//////////////////////////////////////////////////////////////////////////////
double 
Coord3d::dotProduct
  ( const Coord3d & other
  ) const
{
  return x*other.x + y*other.y + z*other.z;
}

//////////////////////////////////////////////////////////////////////////////
ostream & 
jaephys::operator<< 
  ( ostream & os
  , const Coord3d & c
  )
{
  os << "(" << c.x << ", " << c.y << ", " << c.z << ")";
  return os;
}

//////////////////////////////////////////////////////////////////////////////
bool 
Coord3d::operator==( const Coord3d & other) const
{
  return (x == other.x) && (y == other.y) && (z == other.z);
}

//////////////////////////////////////////////////////////////////////////////
// Descripcion: 
//   normaliza el vector de modo que su module sea 1, pero conserve la 
//   direccion y el sentido.
void 
Coord3d::normalize()
{
  double mod = this->module();
  GUARANTEE(!areEqual(mod, 0,0), "Null Module: mod=" << mod);
  this->operator/=(mod);
}

//////////////////////////////////////////////////////////////////////////////
// Descripcion:
//   determina si la coordenada es nula o no ( con un margen de error)
bool Coord3d::isNull(double error) const
{
  return areEqual(x,0,error) && 
         areEqual(y,0,error) && 
         areEqual(z,0,error);
}
