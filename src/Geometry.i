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

using namespace jaephys;

///////////////////////////////////////////////////////////////////////////////
// Intersection Edge-Polygon
///////////////////////////////////////////////////////////////////////////////
inline IntersectionEP::IntersectionEP()
{}

inline IntersectionEP::IntersectionEP( const Coord3D & other_point, 
																			 unsigned int other_index_edge, 
																			 unsigned int other_index_pol )
{
	point = other_point;
	index_edge = other_index_edge;
	index_pol = other_index_pol;
}

inline IntersectionEP &
IntersectionEP::operator=( const IntersectionEP & other)
{
	point = other.point;
	index_edge = other.index_edge;
	index_pol = other.index_pol;

	return *this;
}

///////////////////////////////////////////////////////////////////////////////
// inline functions
///////////////////////////////////////////////////////////////////////////////
inline double 
jaephys::absolute( double value)
{
	if( value < 0)
		return -value;
	return value;
}

// comparisons with error margin
inline bool 
jaephys::areEqual( double a, double b, double error= ERROR_MARGIN)
{
	return absolute(a - b) <= error;
}

inline bool 
jaephys::areEqual(const Coord3D & a, const Coord3D & b)
{
	return areEqual(a.x, b.x) && areEqual(a.y, b.y) && areEqual(a.z, b.z);
}

inline std::ostream & 
jaephys::operator<< ( std::ostream & os, const IntersectionEP & intersec)
{
	os << "( point" << intersec.point 
		 << ", edge " << intersec.index_edge 
		 << ", pol " << intersec.index_pol 
		 << ")";
	return os;
}

template<class D> inline
D & max( D value1, D value2)
{
	if( value1 < value2)
    {
      return value2;
    }
	return value1;
}

inline double 
jaephys::rad2deg( double rad )
{ 
	return (rad * 180.0)/PI;
}

inline double 
jaephys::deg2rad( double grad )
{ 
	return (grad * PI)/180.0;
}

