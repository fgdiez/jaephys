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

#include "ImpulsiveCollision.hpp"
#include "CollisionDetermination.hpp"
#include "Entity.hpp"
#include "Geometry.hpp"
#include "PolygonalShape.hpp"

using namespace jaephys;

// elasticity factor (1: perfect elasticity, 0: perfect impact absortion)
const double EPSILON=1.0; 

ImpulsiveCollision::ImpulsiveCollision( Entity& entity1, 
																				Entity& entity2, 
																				const Collision & collision )
	: collision_(collision)
{
  shape1_ = entity1.findAttribute<Shape,PolygonalShape>("Shape");
  shape2_ = entity2.findAttribute<Shape,PolygonalShape>("Shape");
  
  m1_= entity1.findAttribute<double>( "Mass");
  m2_= entity2.findAttribute<double>( "Mass");
 
  v1_= entity1.findAttribute<Coord3D>( "LinearVelocity");
 	v2_= entity2.findAttribute<Coord3D>( "LinearVelocity");
 
  w1_= entity1.findAttribute<Coord3D>( "AngularVelocity");
  w2_= entity2.findAttribute<Coord3D>( "AngularVelocity");
 
  L1_ = entity1.findAttribute<Coord3D>("AngularMomentum");
  L2_ = entity2.findAttribute<Coord3D>("AngularMomentum");
}

/////////////////////////////////////////////////////////////////////////////
ImpulsiveCollision::~ImpulsiveCollision()
{
}

/////////////////////////////////////////////////////////////////////////////
bool
ImpulsiveCollision::run()
{
  Coord3D r1, r2; 
  r1.assignSubtraction( collision_.collisionPoint1, shape1_->centre() );
  r2.assignSubtraction( collision_.collisionPoint2, shape2_->centre() );

  Coord3D vw1, vw2;
  vw1.assignCrossProduct( *w1_, r1 );
  vw2.assignCrossProduct( *w2_, r2 );

  Coord3D vp1, vp2;
  vp1.assignAddition( *v1_, vw1 );
  vp2.assignAddition( *v2_, vw2 );

  Coord3D vrel_v; 
	vrel_v.assignSubtraction(vp1,vp2);

  const Coord3D & n = collision_.normal;
  GUARANTEE( areEqual( n.module(), 1 ), 
						 "Normal=" << n << " not unitary");

  double vrel = n.dotProduct( vrel_v );
  double numerator= -vrel*( EPSILON + 1 );
 
  double m_1= static_cast<double>(1) / (*m1_);
  double m_2= static_cast<double>(1) / (*m2_);

  Matrix3x3 I1,I2;
  getInertialTensor( I1, shape1_->vPoint_, shape1_->centre(), *m1_);
  getInertialTensor( I2, shape2_->vPoint_, shape2_->centre(), *m2_);

  Matrix3x3 Iinv1, Iinv2;
  getInverseOfSymmetric( I1, Iinv1 );
  getInverseOfSymmetric( I2, Iinv2 );

  Coord3D aux1,aux2; 
  aux1.assignCrossProduct( r1, n );
  multiply( Iinv1, aux1, aux2 );
  aux1.assignCrossProduct( aux2, r1 );
  double term3 = n.dotProduct( aux1 );

  aux1.assignCrossProduct( r2, n );
  multiply( Iinv2, aux1, aux2 );
  aux1.assignCrossProduct( aux2, r2 );
  double term4 = n.dotProduct( aux1 );
  
	double j = numerator / ( m_1 + m_2 + term3 + term4 );

  Coord3D impulse= n;
  impulse *= j;

  // incremento de v1 y v2
  aux1 = impulse;
  aux1 *= m_1;
  (*v1_) += aux1;

  aux2 = impulse;
  aux2 *= m_2;
  (*v2_) -= aux2;

  // Incremento de L1 y L2
  aux1.assignCrossProduct( r1, impulse );
  aux2.assignCrossProduct( r2, impulse );
  (*L1_) += aux1;
  (*L2_) -= aux2;

  // incremento de w1_ y w2
  Coord3D w1f, w2f;
  multiply( Iinv1, (*L1_), w1f );
  multiply( Iinv2, (*L2_), w2f );

  (*w1_) = w1f;
  (*w2_) = w2f;

  return true;
} 
