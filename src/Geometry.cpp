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


#include "Geometry.hpp"
#include "auxiliary.hpp"
#include "debugging.h"

#include <math.h>

using namespace jaephys;

//////////////////////////////////////////////////////////////////////////////
void 
jaephys::getMassCentre( const std::vector<Vertex> & vPoint, Coord3D & centre )
{
  centre.nullify();
  unsigned int size = vPoint.size();
  for(unsigned int i=0; i<size; ++i)
  {
    centre += vPoint[i];
  }
  centre /= size;
  TRACE(1,"centre: " << centre);
}

//////////////////////////////////////////////////////////////////////////////
// Estado: en uso
// Description: haya el point de intersection de un plano( dado por un point
//   y su normal) y una recta( dada por un point y su direction). 
//   Devuelve si hay o no intersection.
//   
bool 
jaephys::intersectPlaneAndLine
  ( const Coord3D & planeNormal
	, const Coord3D & planePoint
  , const Coord3D & lineDirection
  , const Coord3D & linePoint
  , Coord3D & intersection
  )
{
  TRACEC(JAEPHYS_GEO,3,"enter");
  double D = planeNormal.dotProduct(planePoint);

  double t = D - planeNormal.dotProduct(linePoint);
  double divisor = planeNormal.dotProduct(lineDirection);
  double margin=1.0E-4;
  if( areEqual(divisor,0, margin))
  {
    TRACEC(JAEPHYS_GEO,1,"no hay point de intersection");
    return false;
  }
  else
  {
    t /= divisor;
    intersection.x = linePoint.x + lineDirection.x*t;
    intersection.y = linePoint.y + lineDirection.y*t;
    intersection.z = linePoint.z + lineDirection.z*t;
    GUARANTEE
     ( areEqual( 0, planeNormal.dotProduct(intersection) - D, 0.1)
             , "Error!!! Punto de intersecccion no pertenece al plano " <<
                "criterio=" << 
                planeNormal.dotProduct(intersection) - D
             );
  }
  TRACEC(JAEPHYS_GEO,3,"hay point de intersection");
  return true;
}

//////////////////////////////////////////////////////////////////////////////
// Estado: en uso
// Description: determina el point de intersection de un polygon y una edge
//   Devuelve si hay o no intersection.
//   
bool 
jaephys::intersectPolygonAndEdge
  ( Polygon & polygon
  , Edge & edge
  , Coord3D & intersection
  )
{
  const Coord3D & planePoint = (polygon.edge(0)).end1();
  const Coord3D & polygonNormal = polygon.normal();
  const Coord3D & linePoint = edge.end1();
  const Coord3D & lineDirection = (edge.direction());

  bool doIntersect
   = intersectPlaneAndLine
     ( polygonNormal, planePoint
     , lineDirection, linePoint
     , intersection
     );

  if(doIntersect)
  {
    TRACEC(JAEPHYS_GEO,2,"inter=" << intersection << " edge="<< edge);
    doIntersect = false;
    if( edge.contains(intersection, 0.0))
    {
      TRACEC(JAEPHYS_GEO,4,"pi dentro edge");
      if( polygon.contains(intersection, 0.0))
      {
        TRACEC(JAEPHYS_GEO,4,"pi dentro polygon");
        doIntersect = true;
      }
    }
  }
  return doIntersect;
}

//////////////////////////////////////////////////////////////////////////////
// Estado: en uso
// Description: 
//   Comprueba si los polygons tienen una edge comun. De ser asi, la copia
//   en el parametro de salida commonEdge, y devuelve true. En caso contrario
//   devuelve false.
bool
jaephys::getCommonEdge
  ( const Polygon & polygon1
  , const Polygon & polygon2
  , const Edge *& commonEdge
  ) 
{
  unsigned int n1 = polygon1.numberOfEdges();
  unsigned int n2 = polygon2.numberOfEdges();
  
  for( unsigned int i=0; i < n1; ++i)
  {
    for( unsigned int j=0; j < n2; ++j)
    {
      if( polygon1.edge(i) == polygon2.edge(j))
      {
        commonEdge = &(polygon1.edge(i));
        return true;
      }
    }
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////////
// Estado: en uso
// Description: multiplica dos matrices 3x3. Devuelve m1 x m2.
// 
void 
jaephys::multiply
  ( const Matrix3x3 & m1 
  , const Matrix3x3 & m2
  , Matrix3x3 & res
  )
{
  int i,j,k;
  for(i=0; i<3; ++i)
  {
    for(j=0; j<3; ++j)
    { 
      res[i][j] = 0;
      for(k=0; k<3; ++k)
      {
        res[i][j] += (m1[i][k])*(m2[k][j]);
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
// Estado: en uso
// Description:
//   multiplaca una matriz 3x3 con un vector(traspuesto). Devuelve m x v
//   
void 
jaephys::multiply
  ( const Matrix3x3 & m
  , const Coord3D & v
  , Coord3D & res
  )
{
  res.x = v.x*m[0][0] + v.y*m[0][1] + v.z*m[0][2];
  res.y = v.x*m[1][0] + v.y*m[1][1] + v.z*m[1][2];
  res.z = v.x*m[2][0] + v.y*m[2][1] + v.z*m[2][2];
}

//////////////////////////////////////////////////////////////////////////////
// Estado: en uso
// Description: 
//   devuelve la matriz 3x3 traspuesta.
//   
void 
jaephys::assignTransposed
  ( const Matrix3x3 & m
  , Matrix3x3 & res
  )
{
  for(int j=0; j<3; ++j)
    for(int i=0;i<3; ++i)
      res[i][j] = m[j][i];
}

//////////////////////////////////////////////////////////////////////////////
// Estado: en uso
// Description: 
//   forma una lista con los polygons que tienen en comun el vertex "v" y
//   asocia esta a dicho vertex.
//   
void 
jaephys::associatePolygonsToVertex( const std::vector<Polygon> & vPol, Vertex & v)
{
  std::vector<int> viPol;
  bool doContinue= true;
  for(unsigned int i=0; i<vPol.size(); ++i)
  {
    for(unsigned int j=0; doContinue && (j < vPol[i].numberOfEdges()); ++j)
    {
      const Edge & edge = (vPol[i]).edge(j);
      if( v.point == edge.end1())
      {
        doContinue = false;
        viPol.push_back(i);
      }
    } // for lados del polygon
    doContinue = true;
  } // for polygons
  GUARANTEE(viPol.size() > 0,"No association found");
  v.isPartOf(viPol);
}

//////////////////////////////////////////////////////////////////////////////
// Estado: en uso
// Description:
//   determina que par de polygons tienen en comun la edge de entrada.
//   Asocia estos polygons a dicha edge.
//   
void 
jaephys::associatePolygonsToEdge( const std::vector<Polygon> & vPol, Edge & edge)
{
  int i1= -1, i2= -1;
  bool found1=false, found2= false;
  
  for( unsigned int i=0
     ; (!found2) && (i < vPol.size())
     ; ++i
     )
  {
    for( unsigned int j=0
       ; j<vPol[i].numberOfEdges()
       ; ++j
       )
    {
      if( edge == (vPol[i]).edge(j))
      {
        if(found1)
        {
          i2 = i;
          found2= true;
        }
        else
        {
          i1 = i;
          found1 = true;
        }
				break;
      }
    }
  }
  GUARANTEE(found1 && found2, "edge-polygons association failed");
  edge.isPartOf(i1, i2);
}

//////////////////////////////////////////////////////////////////////////////
// Estado: en uso
// Description:
//   devuelve si un point dado, es interior(SI), exterior(NO) o 
//   perteneciente(AMBIGUO) al plano.
//   
Logical 
jaephys::isInsideThePlane
 ( const Coord3D & planeNormal
 , const Coord3D & planePoint
 , const Coord3D & point
 , double error
 )
{
  //CRONO_FUNC_INI;
  // este es un modo mas eficiente de ver si un point esta detras de un plano
  static Coord3D d;  // la declaramos static para no perder tiempo en crear
                     // el objeto cada vez que la llamamos
  d.assignSubtraction(point, planePoint);
  double criteria= planeNormal.dotProduct(d);
  if(areEqual(criteria,0, error))
  {	
    //CRONO_FUNC_FIN;
    return AMBIGUOUS; // el point esta' contenido en el plano del polygon
  }
  if(criteria < 0) {
		//CRONO_FUNC_FIN;
    return YES;
	}
  else {
		//CRONO_FUNC_FIN;
		return NO;
	}
}

//////////////////////////////////////////////////////////////////////////////
// Estado: en uso
// Description:
//   Devuelve la posicion relativa complementaria.
//   
RelativePosition 
jaephys::getComplementary( const RelativePosition pos)
{
  switch(pos)
  {
  case THIS_CONTAINED: return THIS_CONTAINER;
  case THIS_CONTAINER: return THIS_CONTAINED;
  case EXTERNAL: return EXTERNAL;
  default: GUARANTEE(false,"posicion relativa erronea");
  }
}

///////////////////////////////////////////////////////////////////////////////
// Estado: en uso
// Description: calculo del tensor de inercia respecto a un origen de 
//   coordenadas dado
//
void jaephys::getInertialTensor
 ( Matrix3x3 & I
 , const std::vector<Vertex>& lVert
 , const Coord3D& centre
 , const double mass
 )
{
  //CRONO_FUNC_INI;
  unsigned int n= lVert.size();
  double sum_xx=0, sum_yy=0, sum_zz=0;
  double sum_xy=0, sum_xz=0, sum_yz=0;
  double mi = mass/n;

  Coord3D r;
  for(unsigned int i=0; i< n; ++i)
  {
    r.assignSubtraction((lVert[i]).point, centre);

    sum_xx += (r.x)*(r.x);
    sum_yy += (r.y)*(r.y);
    sum_zz += (r.z)*(r.z);
     
    sum_xy += (r.x)*(r.y);
    sum_xz += (r.x)*(r.z);
    sum_yz += (r.y)*(r.z);
  }
  I[0][0] = mi*(sum_yy + sum_zz);
  I[1][1] = mi*(sum_xx + sum_zz);
  I[2][2] = mi*(sum_xx + sum_yy);
  I[0][1] = -mi*sum_xy;
  I[0][2] = -mi*sum_xz;
  I[1][2] = -mi*sum_yz;
  I[1][0] = I[0][1];
  I[2][0] = I[0][2] ;
  I[2][1] = I[1][2];
  //CRONO_FUNC_FIN;
}

std::ostream& jaephys::operator<<( std::ostream & os, Matrix3x3 m )
{
  for(int i=0; i<3; ++i)
  {
    os << "{";
    for(int j=0;j<2; ++j)
      os << m[i][j] << ",";
    os << m[i][2] << "}"<< std::endl;
	}
  return os;
}

void jaephys::resolveSystem( const Matrix3x3 & m, 
														 const Coord3D & v, 
														 Coord3D & s )
{
	Coord3D V(v);
	Matrix3x3 M;	
	copy(m, M);

  if(M[0][0] == 0)
  {
    if(M[1][0] == 0)
    {
      if(M[2][0] == 0)
      {
        GUARANTEE(0, "unresolvable system");
      }
      else
      {
        // interchange row 2 and 0
        for(int i=0;i<3;++i)
        {
					interchange<double>( M[2][i], M[0][i] );
        }
        interchange<double>(V.x, V.z);
      }
    }
    else
    {
      // interchange row 1 and 0
      for(int i=0;i<3;++i)
      {
        interchange<double>( M[1][i], M[0][i] );
      }
			interchange<double>( V.x, V.y);
    }      
  }

  double a=M[1][0]/M[0][0];
  double b=M[2][0]/M[0][0];
  for(int i=1;i<3;++i)
  {
    M[1][i] -= M[0][i]*a;
    M[2][i] -= M[0][i]*b;
  }
  V.y -= V.x*a;
  V.z -= V.x*b;

  if(M[1][1] == 0)
  {
    if(M[2][1] == 0)
    {
      GUARANTEE(0,"unsolvable system");
    }
    else
    {
      // interchange row 1 and 2
      for(int i=0;i<3;++i)
      {
        interchange<double>(M[2][i], M[1][i] );
      }
			interchange<double>( V.y, V.z);
    }
  }

  a= M[2][1]/M[1][1];

  M[2][2] -= M[1][2]*a;
  V.z -= V.y*a;
  
  GUARANTEE(M[2][2] != 0, "unsolvable system");
  s.z = V.z/M[2][2];

  GUARANTEE(M[1][1] != 0, "Error");
  s.y = (V.y - s.z*M[1][2])/M[1][1];
  s.x = (V.x - s.y*M[0][1] -s.z*M[0][2])/M[0][0];
}

void jaephys::getAngularMomentum
 ( const PolygonalShape& ps, const Coord3D& w, const double mass, Coord3D& L
 )
{
  Matrix3x3 I;
  getInertialTensor(I, ps.vPoint_, ps.centre(), mass);
  multiply(I,w, L);
}

void jaephys::assignIdentity(Matrix3x3& m)
{
  for(int i=0; i<2; ++i)
  {
    for(int j=i+1; j<3; ++j)
    {
      m[i][j]= 0;
      m[j][i]= 0;
    }
  }

  m[0][0] = 1; m[1][1]=1; m[2][2]=1;
}

void jaephys::rotateX(const Coord3D& v, double angle, Coord3D& res)
{
  double cosA= cos(angle);
  double senA= sin(angle);
   Matrix3x3 Rx
   ={{ 1,   0,     0}
    ,{ 0, cosA,-senA}
    ,{ 0, senA, cosA}
    };
   multiply(Rx,v,res);
}

void jaephys::rotateY(const Coord3D& v, double angle, Coord3D& res)
{
  double cosA= cos(angle);
  double senA= sin(angle);
   Matrix3x3 Ry
   ={{ cosA,  0, senA}
    ,{    0,  1,    0}
    ,{-senA,  0, cosA}
    };
   multiply(Ry,v,res);
}

void jaephys::rotateZ(const Coord3D& v, double angle, Coord3D& res)
{
  double cosA= cos(angle);
  double senA= sin(angle);
   Matrix3x3 Rz
   ={{ cosA,-senA, 0}
    ,{ senA, cosA, 0}
    ,{ 0,     0,   1}
    };
   multiply(Rz,v,res);
}

void jaephys::copy(const Matrix3x3& matrix, Matrix3x3& copy)
{
  for(int i=0; i<3; ++i)
  for(int j=0; j<3; ++j)
    copy[i][j] = matrix[i][j];
    
}

double jaephys::getDeterminant(const Matrix3x3& a)
{
  return 
    (a[0][0]*a[1][1]*a[2][2]+a[0][1]*a[1][2]*a[2][0]+a[0][2]*a[1][0]*a[2][1]-
    a[0][2]*a[1][1]*a[2][0]-a[0][0]*a[1][2]*a[2][1]-a[0][1]*a[1][0]*a[2][2]);
}

void jaephys::getAdjunctOfSymmetric(const Matrix3x3& m, Matrix3x3& a)
{
  a[0][0]=m[1][1]*m[2][2]-m[1][2]*m[1][2];
  a[1][1]=m[0][0]*m[2][2]-m[0][2]*m[0][2];
  a[2][2]=m[0][0]*m[1][1]-m[0][1]*m[0][1];
  a[0][1]=m[1][2]*m[0][2]-m[0][1]*m[2][2];
  a[0][2]=m[0][1]*m[1][2]-m[1][1]*m[0][2];
  a[1][2]=m[0][1]*m[0][2]-m[0][0]*m[1][2];
  a[1][0]=a[0][1];
  a[2][1]=a[1][2];
  a[2][0]=a[0][2];
}

void jaephys::getInverseOfSymmetric(const Matrix3x3& m, Matrix3x3& i)
{
  getAdjunctOfSymmetric( m, i );
  double div = static_cast<double>(1)/getDeterminant(m);
  multiplyDiagonal( i, div );
}

///////////////////////////////////////////////////////////////////////////////
// Description:
//  Multiply una matriz 3x3 simétrica por un escalar. Se aprovecha la que
//  la matriz es simétrica para optimizar los cálculos.
//
void jaephys::multiplyDiagonal(Matrix3x3& m,double d)
{
  for(int i=0; i<3; ++i)
  {
    for(int j=i; j<3; ++j)
    {
      m[i][j] *= d;
      if(i != j)
        m[j][i]= m[i][j];
    }
  }
}
