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


#include "Coord3D.hpp"
#include "Geometry.hpp"
#include "debugging.h"
#include "auxiliary.hpp"

#include <math.h>

using namespace jaephys;

//////////////////////////////////////////////////////////////////////////////
Coord3D::Coord3D()
 : x(0)
 , y(0)
 , z(0)
{}

//////////////////////////////////////////////////////////////////////////////
Coord3D::Coord3D( const Coord3D & p)
 : x(p.x)
 , y(p.y)
 , z(p.z)
{}

//////////////////////////////////////////////////////////////////////////////
Coord3D::Coord3D( double x0, double y0, double z0)
 : x(x0)
 , y(y0)
 , z(z0)
{} 

void Coord3D::set(double x0, double y0, double z0)
{
	x = x0;
	y = y0;
	z = z0;
}

//////////////////////////////////////////////////////////////////////////////
Coord3D & 
Coord3D::operator=( const Coord3D &p)
{
  x = p.x;
  y = p.y;
  z = p.z;
  return *this;
}

//////////////////////////////////////////////////////////////////////////////
const Coord3D & 
Coord3D::operator+=( const Coord3D & p)
{
  x += p.x;
  y += p.y;
  z += p.z;
  return *this;
}

//////////////////////////////////////////////////////////////////////////////
const Coord3D & 
Coord3D::operator-=( const Coord3D & p)
{
  x -= p.x;
  y -= p.y;
  z -= p.z;
  return *this;
}

//////////////////////////////////////////////////////////////////////////////
const Coord3D & 
Coord3D::operator*=( double factor)
{
  x *= factor;
  y *= factor;
  z *= factor;
  return *this;
}

//////////////////////////////////////////////////////////////////////////////
const Coord3D & 
Coord3D::operator/=( double factor)
{
  double div= static_cast<double>(1)/factor;
  x *= div;
  y *= div;
  z *= div;
  return *this;
}

//////////////////////////////////////////////////////////////////////////////
double 
Coord3D::module() const
{
  return sqrt(x*x + y*y + z*z);
}

//////////////////////////////////////////////////////////////////////////////
void 
Coord3D::assignSubtraction(
  const Coord3D & addend,
  const Coord3D & subtrahend
)
{
  x = addend.x - subtrahend.x;
  y = addend.y - subtrahend.y;
  z = addend.z - subtrahend.z;
}

//////////////////////////////////////////////////////////////////////////////
void 
Coord3D::assignAddition (
  const Coord3D & addend1,
  const Coord3D & addend2
)
{
  x = addend1.x + addend2.x;
  y = addend1.y + addend2.y;
  z = addend1.z + addend2.z;
}

//////////////////////////////////////////////////////////////////////////////
void 
Coord3D::assignCrossProduct
  ( const Coord3D & v1
  , const Coord3D & v2
  )
{
  x = v1.y*v2.z - v1.z*v2.y;
  y = v1.z*v2.x - v1.x*v2.z;
  z = v1.x*v2.y - v1.y*v2.x;
}

//////////////////////////////////////////////////////////////////////////////
void 
Coord3D::changeSign()
{
  x = -x;
  y = -y;
  z = -z;
}

//////////////////////////////////////////////////////////////////////////////
double 
Coord3D::dotProduct
  ( const Coord3D & other
  ) const
{
  return x*other.x + y*other.y + z*other.z;
}

//////////////////////////////////////////////////////////////////////////////
std::ostream & 
jaephys::operator<< ( std::ostream & os, const Coord3D & c )
{
  os << "(" << c.x << ", " << c.y << ", " << c.z << ")";
  return os;
}

//////////////////////////////////////////////////////////////////////////////
bool 
Coord3D::operator==( const Coord3D & other) const
{
  return (x == other.x) && (y == other.y) && (z == other.z);
}

//////////////////////////////////////////////////////////////////////////////
// Description: turn the module into 1
//
void 
Coord3D::normalize()
{
  double mod = this->module();
  GUARANTEE(!areEqual(mod, 0,0), "Null Module: mod=" << mod);
  this->operator/=(mod);
}

//////////////////////////////////////////////////////////////////////////////
// Description: determine if the coordinate is null or not (with an error 
//   margin)
//
bool Coord3D::isNull(double error) const
{
  return areEqual(x,0,error) && 
         areEqual(y,0,error) && 
         areEqual(z,0,error);
}

///////////////////////////////////////////////////////////////////////////////
Coord3D::Coord3D(const std::string & str)
{
  TRACEC(JAEPHYS_ENT,2,"enter");
  unsigned int ini=0;
  unsigned int cont=0;
  unsigned int index= str.find_first_of(',');

  // first coordinate
  cont= index - ini;
  std::string s1( str, ini, cont );  // we get to just before the next comma
	str2Double( s1, x );

  // second coordinate
  ini = index+1;
  index = str.find_first_of(',', ini);
  cont= index - ini;
  std::string s2( str, ini, cont ); // we get to just before the next comma
	str2Double( s2, y );

  // third coordinate
  ini = index+1;
  std::string s3( str, ini, str.size() ); // the rest of the string
	str2Double( s3, z );
}
