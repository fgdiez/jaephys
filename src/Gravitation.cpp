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

#include "Gravitation.hpp"
#include "Entity.hpp"
#include "Shape.hpp"

using namespace jaephys;

///////////////////////////////////////////////////////////////////////////////
Gravitation::Gravitation( Entity& e1, Entity& e2 )
{
  Shape* shape=0;
  shape= e1.findAttribute<Shape>("Shape");
	GUARANTEE(shape, "Shape 1 not found");
  c1_ = &(shape->centre());

  shape= e2.findAttribute<Shape>("Shape");
	GUARANTEE(shape, "Shape 2 not found");
  c2_ = &(shape->centre());

  m1_ = e1.findAttribute<double>("Mass");
	GUARANTEE(m1_, "Mass 1 not found");
  m2_ = e2.findAttribute<double>("Mass");
	GUARANTEE(m2_, "Mass 2 not found");
  a1_ = e1.findAttribute<Coord3D>("Acceleration");
	GUARANTEE(a1_, "Acceleration 1 not found");
  a2_ = e2.findAttribute<Coord3D>("Acceleration");
	GUARANTEE(a2_, "Acceleration 2 not found");
}

///////////////////////////////////////////////////////////////////////////////
Gravitation::~Gravitation()
{
}

///////////////////////////////////////////////////////////////////////////////
bool Gravitation::run()
{
  TRACEC(JAEPHYS_GRAV,1,"entityr");
  // F1: force applied to e1
  // F2: force applied to e2
  // g1: acceleration applied to e1
  // g2: acceleration applied to e2
  // m1: mass of e1
  // m2: mass of e2
  // c1: centre of entity1
  // c2: centre of entity2

  // r = c1 - c2
  // u1 = r/|r| : unitary vector
  // F1 = m1*g1 => g1= F1/m1  
  // F2 = m2*g2 => g2= F2/m2
  // F1 = ((G*m1*m2)/(r²))*u1  : 2ª law of gravitation
  // F1 = -F2
  
  double & m1= *m1_;  
  double & m2= *m2_;
  const double G=6.672e-11;
  TRACEC(JAEPHYS_GRAV,3,"m1="<< m1<< " m2="<< m2);  
  Coord3D u1;
  u1.assignSubtraction(*c2_,*c1_);
  TRACEC(JAEPHYS_GRAV,3,"u1=" << u1);

  if(u1.isNull())
  {
    return true;
  }
  double r_sup2= (u1.module())/pixelsPerMeter;
  r_sup2 *= r_sup2;
  double F= G*m1*m2/r_sup2;
  TRACEC(JAEPHYS_GRAV,2,"F=" << F);
  u1.normalize();
  
  Coord3D g1(u1);
  g1 *= F/m1;
  Coord3D g2(u1);
  g2 *= -F/m2;
  TRACEC(JAEPHYS_GRAV,2,"g1=" << g1);
  TRACEC(JAEPHYS_GRAV,2,"g2=" << g2);

  (*a1_) += g1;
  (*a2_) += g2;
  TRACEC(JAEPHYS_GRAV,1,"a1=" << *a1_);
  TRACEC(JAEPHYS_GRAV,1,"a2=" << *a2_);
  return true;
}
