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


#ifndef JAEPHYS_GEOMETRY_HPP
#define JAEPHYS_GEOMETRY_HPP

#include "auxiliary.hpp"
#include "Coord3D.hpp"
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
    Coord3D point;
    unsigned int index_edge;
    unsigned int index_pol;

    IntersectionEP();
    IntersectionEP( const Coord3D & other_point, 
										unsigned int other_index_edge, 
										unsigned int other_index_pol );

    IntersectionEP & operator=( const IntersectionEP & other);

  }; // IntersectionEP

  std::ostream & operator<< ( std::ostream & os, 
                              const IntersectionEP & intersec );

  double absolute( double value);
  bool areEqual( double a, double b, double error= ERROR_MARGIN);
  bool areEqual( const Coord3D & a, const Coord3D & b);


  bool intersectPlaneAndLine( const Coord3D & planeNormal, 
															const Coord3D & planePoint, 
															const Coord3D & lineDirection, 
															const Coord3D & linePoint, 
															Coord3D & intersection );

  bool intersectPolygonAndEdge( Polygon & polygon, 
																Edge & edge, 
																Coord3D & intersection );
    
  double getAngleClockWise( const Coord3D & vector);

  bool getCommonEdge ( const Polygon & polyg1, 
											 const Polygon & polyg2, 
											 const Edge *& edge );

  double intersectionDepth ( Polygon & pol, 
														 Edge & edge, 
														 const Coord3D & intersection, 
														 const RelativePosition relPos );

  void multiply ( const Matrix3x3 & m1, 
									const Matrix3x3 & m2, 
									Matrix3x3 & result );

  void multiply ( const Matrix3x3 & m, 
									const Coord3D & v, 
									Coord3D & result );

  void assignTransposed( const Matrix3x3 & m, Matrix3x3 & result );

  template<class D> 
	D & max( D value1, D value2);

  void getMassCentre( const std::vector<Vertex> & list, 
											Coord3D & centre );

  void associatePolygonsToVertex( const std::vector<Polygon> & lPolyg, Vertex & v);
  void associatePolygonsToEdge( const std::vector<Polygon> & lPolyg, Edge & edge);

  Logical isInsideThePlane( const Coord3D & planeNormal, 
														const Coord3D & planePoint, 
														const Coord3D & point, 
														double error= ERROR_MARGIN );

  RelativePosition getComplementary( const RelativePosition pos );
  
  unsigned int getMaxColor( NumberOfColors colors );

  bool getHeightAndWidth( GraphicMode mode, 
													unsigned int & height, 
													unsigned int & width );

  void getInertialTensor( Matrix3x3 & I, 
													const std::vector<Vertex> & vPoint, 
													const Coord3D & centre, 
													const double    mass );

	std::ostream& operator<<( std::ostream & os, Matrix3x3 m );

  void resolveSystem( const Matrix3x3 & m, const Coord3D & v, Coord3D & s );

  void getAngularMomentum( const PolygonalShape & shape, 
													 const Coord3D & w, 
													 const double    mass, 
													 Coord3D       & L );

  void assignIdentity(Matrix3x3& m);
  void rotateX(const Coord3D & v, double angle, Coord3D & res);
  void rotateY(const Coord3D & v, double angle, Coord3D & res);
  void rotateZ(const Coord3D & v, double angle, Coord3D & res);
  void copy( const Matrix3x3 & matrix, Matrix3x3 & copy );

  double rad2deg(double rad);
  double deg2rad(double grad);

  void multiplyDiagonal(Matrix3x3 & m, double d);
  double getDeterminant(const Matrix3x3 & a);
  void getAdjunctOfSymmetric(const Matrix3x3 & m, Matrix3x3 & a);
  void getInverseOfSymmetric(const Matrix3x3 & m, Matrix3x3 & i);

} // jaephys

// include inline function definitions
#include "Geometry.i"

#endif // JAEPHYS_GEOMETRY_HPP
