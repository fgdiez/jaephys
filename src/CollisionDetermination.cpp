/******************************************************************************
 * JAEPHYS-ES - Just An Extendable PHYsics Simulator (Spanish version)
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


#include "CollisionDetermination.hpp"
#include "Geometry.hpp"
#include "Entity.hpp"
#include "Polygon.hpp"

#include "debugging.h"

using namespace jaephys;

///////////////////////////////////////////////////////////////////////////////
CollisionDetermination::CollisionDetermination( Entity & e1, 
                                                Entity & e2, 
                                                RelativePosition relPos )
 : w1_(0)
 , w2_(0)
 , relPos1_(relPos)
 , relPos2_(getComplementary(relPos))
{
  shape1_= dynamic_cast< PolygonalShape* >(
      e1.findAttribute< Shape >( "Shape" ) );

  GUARANTEE(shape1_, "Shape 1 not found");
  
  shape2_= dynamic_cast< PolygonalShape* >(
      e2.findAttribute< Shape >( "Shape" ) );

  GUARANTEE(shape2_, "Shape 2 not found");

  v1_= e1.findAttribute<Coord3D>( "LinearVelocity");
	GUARANTEE(v1_, "LinearVelocity 1 not found");
  v2_= e2.findAttribute<Coord3D>( "LinearVelocity");
	GUARANTEE(v2_, "LinearVelocity 2 not found");

	w1_ = e1.findAttribute<Coord3D>("AngularVelocity");
	if(w1_ == 0)
		w1_ = new Coord3D(0,0,0);

	w2_ = e2.findAttribute<Coord3D>("AngularVelocity");
	if(w2_ == 0)
		w2_ = new Coord3D(0,0,0);
}

///////////////////////////////////////////////////////////////////////////////
CollisionDetermination::~CollisionDetermination()
{
}


///////////////////////////////////////////////////////////////////////////////
// RESPONSABILIDAD
//  Coordinar la determinacion de la collision.
//
bool CollisionDetermination::run()
{
  TRACEC(JAEPHYS_COLLID,1,"enter");

  if(relPos1_ == THIS_CONTAINED)
  {
    TRACEC(JAEPHYS_COLLID,2,"Shape1(v1)->Shape2");
    CollisionData cd1;
    cd1.v1= *v1_;
    cd1.w1= *w1_;
    cd1.v2= *v2_;
    cd1.w2= *w2_;
    cd1.shape1= shape1_;
    cd1.shape2= shape2_;
    cd1.relPos= relPos1_;

    bool doCollid1=determineCollision( cd1, collision_);
    if(doCollid1)
    {
      (collision_.normal).normalize();
      repelIntrusion();
    }
    return doCollid1;
  }
  else if(relPos2_ == THIS_CONTAINED)
  {
    TRACEC(JAEPHYS_COLLID,2,"Shape2(v2)->Shape1");
    CollisionData dc2;
    dc2.v1= *v2_;
    dc2.w1= *w2_;
    dc2.v2= *v1_;
    dc2.w2= *w1_;
    dc2.shape1= shape2_;
    dc2.shape2= shape1_;
    dc2.relPos= relPos2_;

    bool doCollid2= determineCollision(dc2, collision_);

    interchange<Coord3D>( collision_.collisionPoint1,
												  collision_.collisionPoint2 );
    if(doCollid2)
    {
      (collision_.normal).normalize();
      repelIntrusion();
    }
    return doCollid2;
  }

  Collision collision1;
  CollisionData cd1;
  cd1.v1= *v1_;
  cd1.w1= *w1_;
  cd1.v2= *v2_;
  cd1.w2= *w2_;
  cd1.shape1= shape1_;
  cd1.shape2= shape2_;
  cd1.relPos= relPos1_;

  Collision collision2;
  CollisionData cd2;
  cd2.v1= *v2_;
  cd2.w1= *w2_;
  cd2.v2= *v1_;
  cd2.w2= *w1_;
  cd2.shape1= shape2_;
  cd2.shape2= shape1_;
  cd2.relPos= relPos2_;

  TRACEC(JAEPHYS_COLLID,2,"Shape1(v1)->Shape2");
  bool doCollid1= determineCollision( cd1, collision1);

  TRACEC(JAEPHYS_COLLID,2,"Shape2(v2)->Shape1");
  bool doCollid2= determineCollision(cd2, collision2);

  interchange<Coord3D>(collision2.collisionPoint1,collision2.collisionPoint2);

  if(!doCollid1)
  {
    TRACEC(JAEPHYS_COLLID,2,"Shape2(-v1)->Shape1");
    (cd1.v1).changeSign();
    (cd1.w1).changeSign();
    (cd1.v2).changeSign();
    (cd1.w2).changeSign();
    cd1.shape1= shape2_;
    cd1.shape2= shape1_;
    cd1.relPos= relPos2_;
    doCollid1= determineCollision( cd1, collision1);

    interchange<Coord3D>(collision1.collisionPoint1,collision1.collisionPoint2);
  }
  if(!doCollid2)
  {
    TRACEC(JAEPHYS_COLLID,2,"Shape1(-v2)->Shape2");
    (cd2.v1).changeSign();
    (cd2.w1).changeSign();
    (cd2.v2).changeSign();
    (cd2.w2).changeSign();
    cd2.shape1= shape1_;
    cd2.shape2= shape2_;
    cd2.relPos= relPos1_;
    doCollid2= determineCollision( cd2, collision2);
  }

  bool doCollid=true;
  if(doCollid1 && doCollid2)
  {
    bool goodVivacity1=checkVivacity(collision1);
    bool goodVivacity2=checkVivacity(collision2);
    if(goodVivacity1 && goodVivacity2)
    {
      if(collision1.penetration >= collision2.penetration){
        TRACEC(JAEPHYS_COLLID,2,"Collision1 choosed");
        collision_= collision1;
      }
      else{
        collision_= collision2;
        TRACEC(JAEPHYS_COLLID,2,"Collision1 choosed");
      }
    }
    else if(goodVivacity1)
    {
      TRACEC(JAEPHYS_COLLID,2,"Entity2 too vivacious");
      TRACEC(JAEPHYS_COLLID,2,"Collision1 choosed");
      collision_= collision1;
    }
    else if(goodVivacity2)
    {
      TRACEC(JAEPHYS_COLLID,2,"Entity1 too vivacious");
      TRACEC(JAEPHYS_COLLID,2,"Collision2 choosed");
      collision_= collision2;
    }
    else
    {
      TRACEC(JAEPHYS_COLLID,2,"Entity1&2 too vivacious");
      return false;
    }
  }// hay collision1 && hay collision2

  else if(doCollid1)
  {
    TRACEC(JAEPHYS_COLLID,2,"Collision1 choosed");
    collision_= collision1;
  }
  else if(doCollid2)
  {
    TRACEC(JAEPHYS_COLLID,2,"Collision2 choosed");
    collision_= collision2;
  }
  else
  {
    TRACEC(JAEPHYS_COLLID,2,"Unexpected: No collision found");
    doCollid=false;
  }


  if(doCollid)
  {
    TRACEC(JAEPHYS_COLLID, 1,"collision=\n" << collision_);
    (collision_.normal).normalize();
    repelIntrusion();
    return true;
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////////
bool CollisionDetermination::determineCollision( CollisionData & cd, 
																								 Collision & collision )
{
  TRACEC(JAEPHYS_COLLID,1,"enter");

  RelativePosition relPos= cd.relPos;

  if(relPos == THIS_CONTAINER)
  {
    TRACEC(JAEPHYS_COLLID,1,"A container do not collid");
    return false;
  }

  if((cd.v1).isNull(0.0001))
  {
    TRACEC(JAEPHYS_COLLID,1, "No collision: null velocity");
    return false;
  } 

  std::vector<Collision> vCollisionVP; // Vertex-Polygon collisions

  determineCollisions_VertexPolygon( cd, vCollisionVP );

  std::vector<Collision> vCollisionEE; // Edge-Edge collisions

  if(relPos1_ == EXTERNAL)
  {
    determineCollisions_EdgeEdge( cd,	vCollisionEE );
  }
  bool doCollid 
   = analizeCandidates( vCollisionVP, vCollisionEE, relPos, collision);
    
  return doCollid;
}

///////////////////////////////////////////////////////////////////////////////
void CollisionDetermination::determineCollisions_VertexPolygon
 ( CollisionData & cd, 
	 std::vector<Collision> & vCollision )
{
  Collision candidate;
  bool doCollid= false;
  Coord3D path, vw,r;
	PolygonalShape & shape1 = *(cd.shape1);
	PolygonalShape & shape2 = *(cd.shape2);

	int size = shape1.vPoint_.size();
  for(int i=0; i < size; ++i)
  {
    candidate.id1 = i;
    const Coord3D & p= shape1.vPoint_[i];

    r.assignSubtraction(p, shape1.centre());
    vw.assignCrossProduct( cd.w1, r );
    path.assignAddition( cd.v1, vw );

    doCollid 
     = determineCollision_VertexPolygon( p, shape2.vPol_, path, cd.v2, cd.w2,
																				 shape2.centre(),cd.relPos, candidate);

    if(doCollid)
    {
      candidate.type= VERTEX_POLYGON;
      vCollision.push_back(candidate);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
void CollisionDetermination::determineCollisions_EdgeEdge
( CollisionData & cd, 
	std::vector<Collision> & vCollision )
{
  Collision candidate;
  bool doCollid;

	unsigned int size = cd.shape1->vPoint_.size();
  for(unsigned int i=0; i< size; ++i)
  {
    candidate.id1 = i;

    doCollid 
     = determineCollision_EdgeEdge
       ( cd.shape1->vEdge_[i], cd.shape2->vEdge_, cd.v1, cd.v2, cd.w2, 
				 cd.shape2->centre(), candidate );

    if(doCollid)
    {
      TRACEC(JAEPHYS_COLLID,2,"Collision candidata=" << candidate);
      candidate.type= EDGE_EDGE;
      vCollision.push_back(candidate);
    }
  }
}


///////////////////////////////////////////////////////////////////////////////
bool CollisionDetermination::determineCollision_VertexPolygon
 ( const Coord3D & vertex
 , std::vector<Polygon>& vPol
 , const Coord3D& path
 , const Coord3D& v2
 , const Coord3D& w2
 , const Coord3D& centre2
 , RelativePosition relPos
 , Collision & collision
 )
{
  Coord3D intersection;
  Coord3D vDistance;
  bool doIntersect;
  collision.penetration = -1;
  
	unsigned int size= vPol.size();
  for(unsigned int i= 0; i<size; ++i)
  {
    Polygon & polygon = vPol[i];
    const Coord3D & planeNormal = polygon.normal();

    bool isInside = isInsideThePlane( polygon.normal(), 
																			(polygon.edge(0)).end1(), 
																			vertex );
   
    if(  ((relPos == EXTERNAL) && (isInside != NO))
      || ((relPos == THIS_CONTAINED) && (isInside != YES))
      )
    {
      TRACEC(JAEPHYS_COLLID,3,"el vertex esta \"tras\" el plano "<< i);
      doIntersect 
       = intersectPlaneAndLine( planeNormal, (polygon.edge(0)).end1(), 
																path, vertex, intersection );
      if(doIntersect)
      {
				TRACEC(JAEPHYS_COLLID,3,"intersection=" << intersection);
        if(polygon.contains(intersection))
        {
          TRACEC(JAEPHYS_COLLID,3,"pi dentro polygon");
          TRACEC(JAEPHYS_COLLID,2,"v1="<<path);
          TRACEC(JAEPHYS_COLLID,2,"normal polygon=" << planeNormal);
          // vemos si el polygon está detras de la path del vertex
          double criterion= path.dotProduct(planeNormal);
          if(  ((relPos!=THIS_CONTAINED) && (criterion < 0)) 
            || ((relPos==THIS_CONTAINED) && (criterion > 0))
            )
          {
            TRACEC(JAEPHYS_COLLID,2,"vertex=" << vertex);
            TRACEC(JAEPHYS_COLLID,2,"polygon=" << i);
            TRACEC(JAEPHYS_COLLID,2,"puntoIntersecPolygon=" << intersection);

            vDistance.assignSubtraction(intersection, vertex);
            double distance = vDistance.module();
            bool goodVivacity
              =checkVivacity
                (path, v2,w2, centre2, intersection
                , distance
                );
            if(goodVivacity)
            {
              if( collision.penetration < distance )
              {
                TRACEC(JAEPHYS_COLLID,3,"distance="<< distance);
                
                collision.penetration = distance;
                collision.collisionPoint2 = intersection;
                collision.normal = planeNormal;
                collision.id2=i;
              }
            } // if vivacidad suficientity
          } // if criterion
        } // if intersection dentro de polygon
      } // if hay interseccion
    } // if vertex esta "detras" del polygon
  } // for
  bool doCollid= (collision.penetration != -1);
  if(doCollid)
  { 
    collision.collisionPoint1 = vertex;
    TRACEC(JAEPHYS_COLLID,1,"collision candidata definitiva:" << std::endl << collision);
  }
  TRACEC(JAEPHYS_COLLID,1,"Hay collision=" << doCollid);
  return doCollid;
}

bool CollisionDetermination::checkVivacity
 ( const Coord3D& vp1
 , const Coord3D& v2
 , const Coord3D& w2
 , const Coord3D& p2
 , const Coord3D& centre2
 , double distance
 )
{
  Coord3D r2; 
  r2.assignSubtraction(p2,centre2);

  double d_max= vp1.module() + v2.module() + (w2.module())*r2.module()+0.1;
  if(d_max < distance){
    TRACEC(JAEPHYS_COLLID,2,"Vivacidad incoherentity:"<<std::endl<<
           "d_max="<< d_max << std::endl<<
           "distance="<<distance);
    TRACEC(JAEPHYS_COLLID,3
          , "vp1="<<vp1.module()<<" v2="<<v2.module()<<" w2="<<w2<<
            " r2="<<r2.module()
          );
    return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool CollisionDetermination::determineCollision_EdgeEdge
 ( Edge& edgeIntrusora, std::vector<Edge>& vEdge
 , const Coord3D& path
 , const Coord3D& v2
 , const Coord3D& w2
 , const Coord3D& centre2
 , Collision & collision
 )
{
  // solo se ejecuta para el caso de objetos EXTERNALES
  // 
  const Coord3D & vEdgeIntrusora = edgeIntrusora.direction();

  // normal al plano de la path y la direction de la edge intrusora
  Coord3D planeNormal1;
  planeNormal1.assignCrossProduct(vEdgeIntrusora, path);
  if(planeNormal1.isNull())
  {
    TRACEC(JAEPHYS_COLLID,2,"path paralela a la edgeIntrusora");
    return false;
  }
  // normal al plano perpendicular al plano 1 y conteniendo el vector de la 
  // path
  Coord3D planeNormal2;
  planeNormal2.assignCrossProduct(planeNormal1, path);

  // normal al plano perpendicular al plano 1 y conteniendo el vector direction
  // de la edge intrusora
  Coord3D planeNormal3;
  planeNormal3.assignCrossProduct(planeNormal1, vEdgeIntrusora);

  const Coord3D & vertex1 = edgeIntrusora.end1();
  const Coord3D & vertex2 = edgeIntrusora.end2();
  Coord3D collisionPoint1, collisionPoint2;

  double distance_candidate;
  double & penetration= collision.penetration;
  penetration = -1;

  Coord3D vDistance;
  Logical logical;
  bool doIntersect = false;

  // iteramos sobre las edges candidatas a ser intersectadas  
  for(unsigned int i=0; i< vEdge.size(); ++i)
  {
    Edge & edge = vEdge[i];
    const Coord3D & vEdge = edge.direction();
    // intersectamos el plano formado por la path de la edgeIntrusora
    // con la recta definida por "edge"
    doIntersect = intersectPlaneAndLine( planeNormal1, vertex1
                       , vEdge, edge.end1()
                       , collisionPoint2
                       );

    if( doIntersect)
    {
      // vemos si el punto queda "detras" de la edge intrusora
      logical = isInsideThePlane
               ( planeNormal3, vertex1
               , collisionPoint2
               , 0.000001
               );
      if( logical != NO)
      {
        // vemos si esta dentro de la edge candidata
        if(edge.contains(collisionPoint2))
        {
          // vemos si el punto esta contenido entre los dos planos que 
          // encauzan la path de los vertexs de la edge intrusora
          Logical res1 
            = isInsideThePlane
              ( planeNormal2, vertex1
              , collisionPoint2
              );
          Logical res2 
            = isInsideThePlane
              ( planeNormal2, vertex2
              , collisionPoint2
              );
          // vemos si edgeIntrusora ha pasado por el punto de collision
          if( res1 != res2)
          {
            // hemos encontrado una collision edge-edge
            // calculamos el punto de collision en la edge intrusora
            doIntersect
             = intersectPlaneAndLine
               ( planeNormal3, vertex1, path, collisionPoint2
               , collisionPoint1
               );
            TRACEC(JAEPHYS_COLLID,2,"otro.Edge=" << i);
            TRACEC(JAEPHYS_COLLID,3,"edgeIntrusora=" << edgeIntrusora);
            GUARANTEE
             ( doIntersect
             , "Anomalia en el algoritmo." << std::endl << 
               "NormalPlano3="<<planeNormal3 << std::endl <<
               "vertex1=" << vertex1 << std::endl << 
               "path=" << path << std::endl <<
               "collisionPoint2=" << collisionPoint2
             );
            TRACEC(JAEPHYS_COLLID,3,"collisionPoint1=" << collisionPoint1);

            // solo conservamos el resultado si la distance de intrusion es
            // menor que la de este candidate
            vDistance.assignSubtraction(collisionPoint2, collisionPoint1);
            distance_candidate = vDistance.module();
            TRACEC(JAEPHYS_COLLID,2,"distance:" << distance_candidate);
            
            bool goodVivacity
              =checkVivacity
                (path, v2,w2, centre2, collisionPoint2
                , distance_candidate
                );
            if(goodVivacity)
            {
              TRACEC(JAEPHYS_COLLID,2,"Normal candidata:" << edge.normal());
              if( distance_candidate > penetration)
              {
                TRACEC(JAEPHYS_COLLID,2,"admitido candidate");
                (collision.normal).assignCrossProduct
                  ( vEdge
                  , vEdgeIntrusora
                  );
                collision.collisionPoint1 = collisionPoint1;
                collision.collisionPoint2 = collisionPoint2;
                penetration = distance_candidate;
                collision.id2 = i;
              }
            }
          } // if edgeIntrusora intersecta edge
        } // if edge contains punto collision
      } // if punto collision "detras"
    } // if hay collision
  } // for indices_edge
  if(penetration  != -1)
  {  
    if(collision.normal.z < 0)
    {
      (collision.normal).changeSign();
    }
    // si la z es nula, elegimos la normal con y positiva
    else if(collision.normal.z == 0)
    {
      if( collision.normal.y < 0)
      {
        (collision.normal).changeSign();
      }
      // si la y es nula, elegimos la normal con la x positiva
      else if(collision.normal.y == 0)
      {
        if(collision.normal.x < 0)
          (collision.normal).changeSign();
      }
    }
    TRACEC(JAEPHYS_COLLID,1,"normal:" << collision.normal);
    TRACEC(JAEPHYS_COLLID,1,"hay collision");
    return true;
  }
  TRACEC(JAEPHYS_COLLID,1,"no hay collision");
  return false;
}

//////////////////////////////////////////////////////////////////////////////
bool CollisionDetermination::analizeCandidates
 ( const std::vector<Collision> & ccVertPolygon, const std::vector<Collision> & ccEdgeEdge
 , const RelativePosition relPos
 , Collision & collision
 )
{
  TRACEC(JAEPHYS_COLLID,3,"ccVertPolygon: " << ccVertPolygon);
  TRACEC(JAEPHYS_COLLID,3,"ccEdgeEdge: " << ccEdgeEdge);
  TRACEC(JAEPHYS_COLLID,2,"relPos: " << relPos);
  double distance = -1;
  unsigned int tam_iaa = ccEdgeEdge.size();
  unsigned int tam_ivp = ccVertPolygon.size();

  if(relPos == EXTERNAL)
  {
    if( (tam_iaa == 0) && (tam_ivp == 0))
    {
      TRACEC(JAEPHYS_COLLID,1,"No hay candidates");
      return false;
    }

    unsigned int indice_candidate=0;
    bool elegidaEdgeEdge= false;
    
    for(unsigned int i=0; i<tam_ivp; ++i)
    {
      const Collision & elemento = ccVertPolygon[i];
      if(distance < elemento.penetration)
      {
        indice_candidate = i;
        distance = elemento.penetration;
      }
    }
    for(unsigned int i=0; i<tam_iaa; ++i)
    {
      const Collision & elemento = ccEdgeEdge[i];
      if(distance < elemento.penetration)
      {
        indice_candidate = i;
        distance = elemento.penetration;
        elegidaEdgeEdge = true;
      }
    }
    if(elegidaEdgeEdge)
    { 
      TRACEC(JAEPHYS_COLLID,2, "Elegida collision edge-edge");
      collision = ccEdgeEdge[indice_candidate];
    }
    else
    { 
      TRACEC(JAEPHYS_COLLID,2, "Elegida collision vertex-polygon");
      collision = ccVertPolygon[indice_candidate];
    }
  }
  else // CONTAINER-CONTAINED
  {
    // solo se tienen en cuenta las intrusiones vertex-polygon
    collision.type= VERTEX_POLYGON;

    if(tam_ivp==0)
    {
      TRACEC(JAEPHYS_COLLID,2,"No hay candidates para collision CONT-CONT");
      return false;
    }

    unsigned int indice_candidate=0;
    for(unsigned int i=0; i<tam_ivp; ++i)
    {
      const Collision & elemento = ccVertPolygon[i];
      if(distance < elemento.penetration)
      {
        indice_candidate = i;
        distance = elemento.penetration;
      }
    }
    collision = ccVertPolygon[indice_candidate];

  } // if CONTAINER-CONTAINED

  TRACEC(JAEPHYS_COLLID,1,"collision: " << collision);
  return true;
}


//////////////////////////////////////////////////////////////////////////////
// RESPONSABILIDAD: separacion de los entitys tras el choque para evitar que se
//   "enganchen"
//
bool CollisionDetermination::repelIntrusion()
{

  TRACEC(JAEPHYS_COLLID,2,"pc1="<<collision_.collisionPoint1);
  TRACEC(JAEPHYS_COLLID,2,"pc2="<<collision_.collisionPoint2);
  Coord3D r;  // vector de intrusion
  r.assignSubtraction(collision_.collisionPoint2, collision_.collisionPoint1);
  TRACEC(JAEPHYS_COLLID,2,"r="<<r);

  if(r.isNull())
  {
    // no estan incrustados, asi que no hace falta separarlos
    return true;
  }

  r.normalize();
  // distance de intrusion
  double r_mod = r.module();
  r_mod += 1; // nos aseguramos de que los objetos se separarán
  
  switch(relPos1_){
  case EXTERNAL:
  {
    double v1= (*v1_).module();
    double v2= (*v2_).module();
    double div=static_cast<double>(1)/(v1+v2);
    Coord3D r1(r);
    r1 *= (v1 * r_mod * div);
    TRACEC(JAEPHYS_COLLID,2,"Vec repulsion1="<<r);
    shape1_->translate(r1);
    r.changeSign();
    Coord3D r2(r);
    r2 *= (v2 * r_mod * div);
    TRACEC(JAEPHYS_COLLID,2,"Vec repulsion2="<<r);
    shape2_->translate(r2);
    break;
  }
  case THIS_CONTAINED:
  {
    // translatemos, el entity contenido, la distance de intrusion
    r *= r_mod;
    shape1_->translate(r);
    break;
  }
  case THIS_CONTAINER:
  {
    // translatemos, el entity contenido, la distance de incrustacion + 1
    r *= -r_mod;
    shape2_->translate(r);
    break;
  }
  default:
    GUARANTEE(false, "Tipo de posicion incorrecta");
  } // switch

  return true;
}

///////////////////////////////////////////////////////////////////////////////
// RESPONSABILIDADES
// + Comprobar que la distance de intrusion es coherentity respecto a la máxima
//   variación de las posiciones que han podido sufrir los entitys
///////////////////////////////////////////////////////////////////////////////
bool CollisionDetermination::checkVivacity(Collision& collision)
{
  // deteccion de objetos "enganchados"
  if( relPos1_ == EXTERNAL)
  {
    // guaranteemos que son coherentitys los puntos de collision y la distance
    // que hay entre ellos (solo si se trata de objetos exteriores)

    // vector de intrusion
    Coord3D r;
    r.assignSubtraction(collision.collisionPoint2, collision.collisionPoint1);

    // distance de intrusion
    double r_mod = r.module();

    GUARANTEE
     ( areEqual(r_mod, collision.penetration)
     , "r_mod="<< r_mod<< " distI="<< collision.penetration
     );

    // hayamos la maxima traslacion posible de los entitys, respecto a la 
    // situacion anterior al choque
    // 
    Coord3D r1(collision.collisionPoint1);
    r1 -= shape1_->centre();
    Coord3D r2(collision.collisionPoint2);
    r2 -= shape2_->centre();
    double vw1= w1_->module()*shape1_->outerRadius();
    double vw2= w2_->module()*shape2_->outerRadius();
    double vivacidad = v1_->module()+ v2_->module()+ vw1 + vw2 +1;
    if( r_mod > vivacidad)
    {
      // la collision detectada es anomala, pues el module de r es mayor que la 
      // suma de las velocidades,suponemos que lo que hace falta es dejar que se
      // separen dejando que continuen su path.
      TRACEC( JAEPHYS_COLLID,2, "Vivacidad anomala: "<< 
              "r_mod="<< r_mod<< " vivacidad="<< vivacidad
             );
      return false;
    }
  } // if(relPos1_==EXTERNAL)

  return true;
}

bool CollisionDetermination::checkCollision
 ( CollisionData & cd
 , Collision& collision
 )
{
  bool doCollid=false;
  if(collision.type == VERTEX_POLYGON){
    const Vertex& vert= (cd.shape1)->vPoint_[collision.id1];
    Polygon& polygon= (cd.shape2)->vPol_[collision.id2];
    doCollid = checkCollision_VertexPolygon( vert.point, polygon, cd.v1, 
																						 cd.relPos );
  }
  else if(collision.type == EDGE_EDGE){
    Edge& edge1= (cd.shape1)->vEdge_[collision.id1];
    Edge& edge2= (cd.shape2)->vEdge_[collision.id2];
    doCollid= checkCollision_EdgeEdge(edge1, edge2, cd.v1);
  }
  else
    GUARANTEE(false,"El tipo de Collision es incorrecto");

  return doCollid;
}



///////////////////////////////////////////////////////////////////////////////
bool CollisionDetermination::checkCollision_VertexPolygon
 ( const Coord3D & vertex
 , Polygon& polygon
 , const Coord3D& path
 , RelativePosition relPos
 )
{
  Coord3D intersection;
  bool doCollid= false;
  bool doIntersect= false;

  const Coord3D & planeNormal = polygon.normal();

  // vemos si el point de interseccion esta "detras" del plano del polygon
  Logical logical 
    = isInsideThePlane 
      ( polygon.normal()
      , (polygon.edge(0)).end1()
      , vertex
      );

  // la logica se invierte dependiendo de si se trata de objetos exteriores o 
  // contenedor/contenido
  if(  ((relPos == EXTERNAL) && (logical != NO))
    || ((relPos == THIS_CONTAINED) && (logical != YES))
    )
  {
    doIntersect 
      = intersectPlaneAndLine
         ( planeNormal, (polygon.edge(0)).end1()
         , path, vertex
         , intersection
         );
    TRACEC(JAEPHYS_COLLID,3,"intersection=" << intersection);
    if(doIntersect)
    {
      // vemos si el point de interseccion esta dentro del polygon
      doCollid= polygon.contains(intersection);
    } // if hay interseccion
  } // if vertex esta "detras" del polygon

  return doCollid;
}

//////////////////////////////////////////////////////////////////////////////
bool CollisionDetermination::checkCollision_EdgeEdge
 ( Edge& edgeIntrusora
 , Edge& edge
 , const Coord3D& path
 )
{
  // solo se ejecuta para el caso de objetos EXTERNALES
  // 
  const Coord3D & vEdgeIntrusora = edgeIntrusora.direction();

  // normal al plano de la path y la direction de la edge intrusora
  Coord3D planeNormal1;
  planeNormal1.assignCrossProduct(vEdgeIntrusora, path);
  if(planeNormal1.isNull())
  {
    TRACEC(JAEPHYS_COLLID,2,"path paralela a la edgeIntrusora");
    return false;
  }
  // normal al plano perpendicular al plano 1 y conteniendo el vector de la 
  //path
  Coord3D planeNormal2;
  planeNormal2.assignCrossProduct(planeNormal1, path);

  // normal al plano perpendicular al plano 1 y conteniendo el vector direction
  // de la edge intrusora
  Coord3D planeNormal3;
  planeNormal3.assignCrossProduct(planeNormal1, vEdgeIntrusora);

  const Coord3D & vertex1 = edgeIntrusora.end1();
  const Coord3D & vertex2 = edgeIntrusora.end2();

  Logical logical;
  bool doIntersect = false;

  const Coord3D & vEdge = edge.direction();

  Coord3D collisionPoint;
  // intersectamos el plano formado por la path de la edgeIntrusora
  // con la recta definida por "edge"
  doIntersect = intersectPlaneAndLine
                     ( planeNormal1, vertex1
                     , vEdge, edge.end1()
                     , collisionPoint
                     );

  if( doIntersect)
  {
    // vemos si el point queda "detras" de la edge intrusora
    logical = isInsideThePlane
             ( planeNormal3, vertex1
             , collisionPoint
             , 0.000001
             );
    if( logical != NO)
    {
      // vemos si esta dentro de la edge candidata
      if(edge.contains(collisionPoint))
      {
        // vemos si el point esta contenido entre los dos planos que 
        // encauzan la path de los vertexs de la edge intrusora
        Logical res1 
          = isInsideThePlane
            ( planeNormal2, vertex1
            , collisionPoint
            );
        Logical res2 
          = isInsideThePlane
            ( planeNormal2, vertex2
            , collisionPoint
            );
        // vemos si edgeIntrusora ha pasado por el point de collision
        if( res1 != res2)
        {
          // hemos encontrado una collision edge-edge
          return true;
        }
      } // if edge contains point collision
    } // if point collision "detras"
  } // if hay interseccion

  return false;
}

