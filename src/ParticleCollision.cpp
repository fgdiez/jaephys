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

#include "ParticleCollision.hpp"
#include "Entity.hpp"
#include "Geometry.hpp"
#include "Attribute.hpp"

#include <math.h>

using namespace jaephys;

///////////////////////////////////////////////////////////////////////////////
ParticleCollision::ParticleCollision( Entity & entity1, 
																			Entity & entity2, 
																			const Coord3D& normal )
	: n_(normal)
{
  v1_ = entity1.findAttribute<Coord3D>( "LinearVelocity");
	GUARANTEE(v1_, "LinearVelocity 1 not found");
  v2_ = entity2.findAttribute<Coord3D>( "LinearVelocity");
	GUARANTEE(v2_, "LinearVelocity 2 not found");

  m1_ = entity1.findAttribute<double>( "Mass");
	GUARANTEE(m1_, "Mass 1 not found");
  m2_ = entity2.findAttribute<double>( "Mass");
	GUARANTEE(m2_, "Mass 2 not found");
}

/////////////////////////////////////////////////////////////////////////////
ParticleCollision::~ParticleCollision()
{
}

/////////////////////////////////////////////////////////////////////////////
bool
ParticleCollision::run()
{
  Coord3D& v1= *v1_;
  Coord3D& v2= *v2_;

  TRACEC(JAEPHYS_COLLID,1, "v1(ini)=" << v1 << std::endl << "v2(ini)=" << v2);
  TRACEC(JAEPHYS_COLLID,1,"collision normal=" << n_);
  GUARANTEE(! areEqual(n_.module(), 0), "n_=" << n_ << " is null");

  double v1n = n_.dotProduct(v1);
  double v2n = n_.dotProduct(v2);
  TRACE(3, "v1n: " << v1n);
  TRACE(3, "v2n: " << v2n);

  double v1n_res, v2n_res;
  resolveFrontalCollision( v1n, v2n, v1n_res, v2n_res );
  TRACE(3,"v1n_res: " << v1n_res);
  TRACE(3,"v2n_res: " << v2n_res);
  
  double v1np, v2np;
  Coord3D np1, np2, aux;
  aux.assignCrossProduct( n_, v1 );
  TRACEC( JAEPHYS_COLLID, 3, "aux: " << aux);
  if( aux.isNull())
  {  
    v1np = 0;
  }
  else
  {
    np1.assignCrossProduct( aux, n_);
    GUARANTEE(!areEqual( np1.module(), 0 ), "np1=" << np1 << " is null");
    np1.normalize();
    v1np = np1.dotProduct( v1 );
    TRACEC(JAEPHYS_COLLID, 3, "np1: " << np1);
    TRACEC(JAEPHYS_COLLID, 3, "v1np: " << v1np);
  }

  aux.assignCrossProduct( n_, v2);
  TRACEC(JAEPHYS_COLLID,3, "aux: " << aux);
  if( aux.isNull())
  {  
    v2np = 0;
  }
  else
  {
    np2.assignCrossProduct( aux, n_);
    GUARANTEE(!areEqual(np2.module(), 0), "np2=" << np2 << " is null");
    np2.normalize();
    v2np = np2.dotProduct(v2);
    TRACE(3, "np2: " << np2);
    TRACE(3, "v2np: " << v2np);
  }
  
  v1.x = v1n_res*n_.x + v1np*np1.x;
  v1.y = v1n_res*n_.y + v1np*np1.y;
  v1.z = v1n_res*n_.z + v1np*np1.z;
 
  v2.x = v2n_res*n_.x + np2.x*v2np;
  v2.y = v2n_res*n_.y + np2.y*v2np;
  v2.z = v2n_res*n_.z + np2.z*v2np;

  TRACEC(JAEPHYS_COLLID,1, "v1=" << v1 << std::endl << "v2=" << v2);
  
  return true;
} 

void ParticleCollision::resolveFrontalCollision( const double v1, 
																								 const double v2, 
																								 double & v1_res, 
																								 double & v2_res )
{
  const double & m1= *m1_;
  const double & m2= *m2_;
  
  if(m1 == m2)
  {
    v1_res = v2;
    v2_res = v1;
  }
  else
  {
    double m1m2_inv = static_cast<double>(1.0)/(m1 + m2);
    double m1m2_inv2 = m1m2_inv*2;
    double ownWeight = (m1 - m2)*m1m2_inv;
    double otherWeight1 = m2*m1m2_inv2;
    double otherWeight2 = m1*m1m2_inv2;
 
    v1_res =   ownWeight*v1 + otherWeight1*v2;
    v2_res = (-ownWeight)*v2 + otherWeight2*v1;
  }

  // momentum preservation
  GUARANTEE( areEqual(v1*m1 + v2*m2, v1_res*m1 + v2_res*m2), 
						 "Linear Momentum not preserved" );
}
