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


#include "PolygonalShape.hpp"
#include "Geometry.hpp"
#include "debugging.h"

#include <math.h>

using namespace jaephys;

//////////////////////////////////////////////////////////////////////////////
PolygonalShape::PolygonalShape( const PolygonalShape& other)
{
  TRACEC(JAEPHYS_SHAPE,1,"entra");

  vPoint_ = other.vPoint_;
	for(unsigned int i=0; i< vPoint_.size(); ++i)
		vPoint_[i].polygons(&vPol_);

  vEdge_ = other.vEdge_;
	for(unsigned int i=0; i< vEdge_.size(); ++i)
		vEdge_[i].vertexes(&vPoint_);

  vPol_ = other.vPol_;

  outerRadius_= other.outerRadius_;
  outerRadiusE2_= other.outerRadiusE2_;
  innerRadius_= other.innerRadius_;
  innerRadiusE2_= other.innerRadiusE2_;
  centre_ = other.centre_;
}

//////////////////////////////////////////////////////////////////////////////
PolygonalShape::~PolygonalShape()
{
  TRACEC(JAEPHYS_SHAPE,1,"entra");
}

//////////////////////////////////////////////////////////////////////////////
PolygonalShape::PolygonalShape()
{
  TRACEC(JAEPHYS_SHAPE,1,"entra");
}

//////////////////////////////////////////////////////////////////////////////
void PolygonalShape::translate( const Coord3d & vector)
{
  TRACEC(JAEPHYS_SHAPE,2,"vector="<< vector);
  centre_ += vector;  

  unsigned int size= vPoint_.size();
  for( unsigned int i= 0; i< size; ++i)
  {
    vPoint_[i].point += vector;
  }  
}

//////////////////////////////////////////////////////////////////////////////
// Description:
//   Rota la forma polygonal en torno a un eje de rotacion dado, el angle
//   indicado.
//   
void PolygonalShape::rotate( const Coord3d & er, const double angle)
{
  Coord3d j(er);
  if(j.isNull(1e-5))
    return;

  j.normalize();
  Coord3d j0(0,1,0);
  Coord3d i,k;
  if(areEqual(j,j0))
  {
    i.x=1; i.y=0; i.z=0;
    k.x=0; k.y=0; k.z=1;
  }
  else
  {
    j0.y = -1;
    if(areEqual(j,j0))
    {
      i.x=1; i.y=0; i.z=0;
      k.x=0; k.y=0; k.z=-1;
    }
    else
    {
      k.assignCrossProduct(j,j0);
      GUARANTEE(k.isNull()==false,"k is null");
      k.normalize();
      i.assignCrossProduct(j,k);
    }
  }
  Matrix3x3 Rw={{i.x,i.y,i.z},{j.x,j.y,j.z},{k.x,k.y,k.z}};
  Matrix3x3 Ry
    ={{cos(angle),0,sin(angle)},{0,1,0},{-sin(angle),0,cos(angle)}};
  Matrix3x3 Rwt;
  assignTransposed(Rw,Rwt);
  Matrix3x3 R,Raux;
  multiply(Ry,Rw,Raux);
  multiply(Rwt,Raux,R);

  unsigned int tam = vPoint_.size();
  Coord3d v;

  for(unsigned int i=0; i< tam; ++i)
  {
    (vPoint_[i]).point -= centre_;
    multiply(R,vPoint_[i],v);
    ((vPoint_[i]).point).assignAddition( v, centre_);
  }
}

///////////////////////////////////////////////////////////////////////////////
// Description:
//   le da la forma de un cubo.
//   
void PolygonalShape::createCube( unsigned int side)
{
  int d = side/2;

  // Inicializacion de la lista de vertexes
  vPoint_= vector<Vertex>(8,Vertex(&vPol_));
  vPoint_[0].point.set(-d, d,-d); // esq izq sup trasera
  vPoint_[1].point.set( d, d,-d); // esq der sup trasera
  vPoint_[2].point.set( d,-d,-d); // esq der inf trasera
  vPoint_[3].point.set(-d,-d,-d); // esq izq inf trasera
  vPoint_[4].point.set(-d, d, d); // esq izq sup delantera
  vPoint_[5].point.set( d, d, d); // esq der sup delantera
  vPoint_[6].point.set( d,-d, d); // esq der inf delantera
  vPoint_[7].point.set(-d,-d, d); // esq izq inf delantera

  // Inicializacion de la lista de edges
  vEdge_= vector<Edge>(12, Edge(&vPoint_));
  vEdge_[0].ends(0, 1);
  vEdge_[1].ends(1, 2);
  vEdge_[2].ends(2, 3);
  vEdge_[3].ends(3, 0);
  vEdge_[4].ends(0, 4);
  vEdge_[5].ends(1, 5);
  vEdge_[6].ends(2, 6);
  vEdge_[7].ends(3, 7);
  vEdge_[8].ends(4, 5);
  vEdge_[9].ends(5, 6);
  vEdge_[10].ends(6, 7);
  vEdge_[11].ends(7, 4);

  // Inicializacion de la lista de polygons del objeto
  //
  vPol_= vector<Polygon>(6,Polygon());

  // cara trasera
	vPol_[0].edges()= vector<Edge>(4, &vPoint_);
  vPol_[0].edge(0).ends(0, 1);
  vPol_[0].edge(1).ends(1, 2);
  vPol_[0].edge(2).ends(2, 3);
  vPol_[0].edge(3).ends(3, 0);

  // cara lateral derecha
	vPol_[1].edges()= vector<Edge>(4, &vPoint_);
  vPol_[1].edge(0).ends(1, 5);
  vPol_[1].edge(1).ends(5, 6);
  vPol_[1].edge(2).ends(6, 2);
  vPol_[1].edge(3).ends(2, 1);

  // cara delantera
	vPol_[2].edges()= vector<Edge>(4, &vPoint_);
  vPol_[2].edge(0).ends(4, 7);
  vPol_[2].edge(1).ends(7, 6);
  vPol_[2].edge(2).ends(6, 5);
  vPol_[2].edge(3).ends(5, 4);

  // cara lateral izquierda
	vPol_[3].edges()= vector<Edge>(4, &vPoint_);
  vPol_[3].edge(0).ends(4, 0);
  vPol_[3].edge(1).ends(0, 3);
  vPol_[3].edge(2).ends(3, 7);
  vPol_[3].edge(3).ends(7, 4);

  // cara superior
	vPol_[4].edges()= vector<Edge>(4, &vPoint_);
  vPol_[4].edge(0).ends(4, 5);
  vPol_[4].edge(1).ends(5, 1);
  vPol_[4].edge(2).ends(1, 0);
  vPol_[4].edge(3).ends(0, 4);

  // cara inferior
	vPol_[5].edges()= vector<Edge>(4, &vPoint_);
  vPol_[5].edge(0).ends(3, 2);
  vPol_[5].edge(1).ends(2, 6);
  vPol_[5].edge(2).ends(6, 7);
  vPol_[5].edge(3).ends(7, 3);

  createAssociations();

  outerRadiusE2_ = d*d*3;   // radio exterior elevado a 2
  outerRadius_ = d*sqrt(3); // radio exterior propiamente dicho
  innerRadiusE2_ = d*d;     // radio interior al cuadrado
  innerRadius_ = d;         // radio interior propiamente dicho
  centre_.set(0,0,0);   // posicion del centre del objeto
}

void PolygonalShape::createAssociations()
{
  // asociamos los polygons con los vertexes
  for(unsigned int i=0; i< vPoint_.size(); ++i)
  {
    associatePolygonsToVertex(vPol_, vPoint_[i]);
  }
  for(unsigned int i=0; i<vEdge_.size(); ++i)
  {
    associatePolygonsToEdge(vPol_, vEdge_[i]);
  }
}


//////////////////////////////////////////////////////////////////////////////
// Description:
//   le da la forma de una piramide
//   
void 
PolygonalShape::createPiramid
 ( unsigned int baseDiagonal
 , unsigned int height
 )
{
  int d = baseDiagonal/2;
  vPoint_= vector<Vertex>(5, Vertex(&vPol_));
  vPoint_[0].point.set(-d, 0, 0); // izq 
  vPoint_[1].point.set( 0, 0,-d); // detras 
  vPoint_[2].point.set( d, 0, 0); // derecha
  vPoint_[3].point.set( 0, 0, d); // delante
  vPoint_[4].point.set( 0, height, 0); // arriba

  vEdge_= vector<Edge>(8, Edge(&vPoint_));
  vEdge_[0].ends( 0, 1); // NO
  vEdge_[1].ends( 1, 2); // NE
  vEdge_[2].ends( 2, 3); // SE
  vEdge_[3].ends( 3, 0); // SO
  vEdge_[4].ends( 0, 4); // O
  vEdge_[5].ends( 1, 4); // N
  vEdge_[6].ends( 2, 4); // E
  vEdge_[7].ends( 3, 4); // S

  vPol_= vector<Polygon>(5,Polygon());

  // base
	vPol_[0].edges()= vector<Edge>(4, Edge(&vPoint_));
  vPol_[0].edge(0).ends(0, 1);
  vPol_[0].edge(1).ends(1, 2);
  vPol_[0].edge(2).ends(2, 3);
  vPol_[0].edge(3).ends(3, 0);

  // cara lateral SE
	vPol_[1].edges()= vector<Edge>(3, Edge(&vPoint_));
  vPol_[1].edge(0).ends(2, 4);
  vPol_[1].edge(1).ends(4, 3);
  vPol_[1].edge(2).ends(3, 2);

  // cara trasera NE
	vPol_[2].edges()= vector<Edge>(3, Edge(&vPoint_));
  vPol_[2].edge(0).ends(1, 4);
  vPol_[2].edge(1).ends(4, 2);
  vPol_[2].edge(2).ends(2, 1);

  // cara lateral NO
	vPol_[3].edges()= vector<Edge>(3, Edge(&vPoint_));
  vPol_[3].edge(0).ends(0, 4);
  vPol_[3].edge(1).ends(4, 1);
  vPol_[3].edge(2).ends(1, 0);

  // cara lateral SO
	vPol_[4].edges()= vector<Edge>(3, Edge(&vPoint_));
  vPol_[4].edge(0).ends(3, 4);
  vPol_[4].edge(1).ends(4, 0);
  vPol_[4].edge(2).ends(0, 3);

  createAssociations();

  // calculamos el centre de la forma
  setGeometricCentre(vPoint_, centre_);

  // calculamos el radio exterior
	double c= baseDiagonal/2.0;
	double alfa= atan(c/height);
	double h2= sqrt(height*height+ c*c)/2.0;
	outerRadius_= h2/cos(alfa);
	outerRadiusE2_= outerRadius_*outerRadius_;

  innerRadius_ = sqrt(outerRadiusE2_- h2*h2);
  innerRadiusE2_ = innerRadius_*innerRadius_;
  TRACEC(JAEPHYS_SHAPE,3,"outerRadius=" << outerRadius_);
  TRACEC(JAEPHYS_SHAPE,3,"innerRadius=" << innerRadius_);
}
