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


#ifndef JAEPHYS_GEOMETRY_HPP
#define JAEPHYS_GEOMETRY_HPP

#include "const_enum_typedef.h"
#include "Coord3d.hpp"
#include "Vertex.hpp"
#include "Edge.hpp"
#include "Polygon.hpp"
#include "PolygonalShape.hpp"

namespace jaephys
{
  // intersection between Edge and Polygon
  // 
  struct IntersectionEP
  {
    Coord3d point;
    unsigned int index_edge;
    unsigned int index_pol;

    IntersectionEP(){};
    IntersectionEP
      ( const Coord3d & other_point
      , unsigned int other_index_edge
      , unsigned int other_index_pol
      )
    {
      point = other_point;
      index_edge = other_index_edge;
      index_pol = other_index_pol;
    }

    IntersectionEP &
    operator=( const IntersectionEP & other)
    {
      point = other.point;
      index_edge = other.index_edge;
      index_pol = other.index_pol;

      return *this;
    }

  }; // IntersectionEP

  inline double absolute( double value)
  {
    if( value < 0)
      return -value;
    return value;
  }

  // comparations with error margin
  inline bool areEqual( double a, double b, double error= ERROR_MARGIN)
  {
    return absolute(a - b) <= error;
  }
  inline bool areEqual(const Coord3d & a, const Coord3d & b)
  {
    return areEqual(a.x, b.x) && areEqual(a.y, b.y) && areEqual(a.z, b.z);
  }

  inline ostream & 
  operator<< ( ostream & os, const IntersectionEP & intersec)
  {
      os << "( point" << intersec.point 
         << ", edge " << intersec.index_edge 
         << ", pol " << intersec.index_pol 
         << ")";
      return os;
  }

  void reflectAgainstPlane( const Coord3d & planeNormal, Coord3d & vector);

  bool 
  intersectPlaneAndLine
   ( const Coord3d & planeNormal
   , const Coord3d & planePoint
   , const Coord3d & lineDirection
   , const Coord3d & linePoint
   , Coord3d & intersection
   );

  bool 
  intersectPolygonAndEdge
    ( Polygon & polygon
    , Edge & edge
    , Coord3d & intersection
    );
    
  double getAngleClockWise( const Coord3d & vector);

  bool
  getCommonEdge
    ( const Polygon & polyg1
    , const Polygon & polyg2
    , const Edge *& edge
    );

  double 
  intersectionDepth
    ( Polygon & pol
    , Edge & edge 
    , const Coord3d & intersection
    , const RelativePosition relPos
    );

  void multiply
    ( const Matrix3x3 & m1 
    , const Matrix3x3 & m2
    , Matrix3x3 & result
    );

  void multiply
    ( const Matrix3x3 & m 
    , const Coord3d & v
    , Coord3d & result
    );

  void assignTransposed(const Matrix3x3 & m, Matrix3x3 & result);

  template<class D>
  D & max( D value1, D value2)
  {
    if( value1 < value2)
    {
      return value2;
    }
    return value1;
  }

  void setGeometricCentre
   ( const vector<Vertex> & list
   , Coord3d & centre
   );

  void associatePolygonsToVertex( vector<Polygon> & lPolyg, Vertex & v);
  void associatePolygonsToEdge( vector<Polygon> & lPolyg, Edge & edge);

  Logical isPlaneInsidePoint
   ( const Coord3d & planeNormal
   , const Coord3d & planePoint
   , const Coord3d & point
   , double error= ERROR_MARGIN
   );

  RelativePosition daComplementario( const RelativePosition pos);
  
  unsigned int getMaxColor( NumberOfColors colors);

  bool getHeightAndWidth
   ( GraphicMode mode
   , unsigned int & height, unsigned int & width
   );

  // (absolute coordinates) 
  void calculateInertialTensor
   ( Matrix3x3 & I
   , const vector<Vertex>& lVert
   , const Coord3d& centre
   , const double mass
   );
	ostream& operator<<(ostream & os, Matrix3x3 m);

  void resolveSystem(Matrix3x3 m, Coord3d v, Coord3d &s);
  void calculateAngularMomentum
   ( const PolygonalShape& fp
   , const Coord3d& w
   , const double mass
   , Coord3d& L
   );

  void assignIdentity(Matrix3x3& m);
  void rotateX(const Coord3d& v, double angulo, Coord3d& res);
  void rotateY(const Coord3d& v, double angulo, Coord3d& res);
  void rotateZ(const Coord3d& v, double angulo, Coord3d& res);
  void copy(const Matrix3x3& matrix, Matrix3x3& copy);

  inline double rad2deg(double rad){return (rad*180)/PI;}
  inline double deg2rad(double grad){return (grad*PI)/180;}

  void multiplySymetrical(Matrix3x3& m,double d);
  double getDeterminant(const Matrix3x3& a);
  void inverseSymetrical(const Matrix3x3& m, Matrix3x3& i);
  void adjunctSumetrical(const Matrix3x3& m, Matrix3x3& a);

} // jaephys

#endif // JAEPHYS_GEOMETRY_HPP
