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


#include "CollisionDetection.hpp"
#include "Entity.hpp"
#include "Geometry.hpp"
#include "debugging.h"

using namespace jaephys;


///////////////////////////////////////////////////////////////////////////////
CollisionDetection::~CollisionDetection()
{
}

///////////////////////////////////////////////////////////////////////////////
CollisionDetection::CollisionDetection( Entity& entity1, 
                                        Entity& entity2, 
                                        RelativePosition relativePos1 )
 : relativePos1_(relativePos1)
{
  shape1_ = entity1.findAttribute<Shape, PolygonalShape>( "Shape");
	GUARANTEE(shape1_, "Shape1 not found");
  shape2_ = entity2.findAttribute<Shape, PolygonalShape>( "Shape");
	GUARANTEE(shape2_, "Shape2 not found");
}

///////////////////////////////////////////////////////////////////////////////
bool 
CollisionDetection::run()
{
  TRACEC(JAEPHYS_COLLID,2,"enter");
  bool detected;

  detected= detectSphericalPenetration();
  if(! detected)
	{
    //CRONO_FUNC_FIN;
		return false;
	}

  detected = detectEdgePenetration(shape1_->vEdge_, shape2_->vPol_);
  TRACECOND( JAEPHYS_COLLID, detected, 
             "intrusion ("<< shape1_->id() << ","<<shape2_->id()<<")" );

  if(!detected)
  {
    detected= detectEdgePenetration(shape2_->vEdge_, shape1_->vPol_);
    TRACECOND( JAEPHYS_COLLID, detected, 
               "intrusion ("<< shape2_->id() << ","<<shape1_->id()<<")" );
  }

  TRACEC(JAEPHYS_COLLID,2,"detected=" << detected);
  return detected;
}

///////////////////////////////////////////////////////////////////////////////
bool 
CollisionDetection::detectSphericalPenetration()
{
  static Coord3D d; // distance vector
  d.assignSubtraction( shape1_->centre(), shape2_->centre());
  
  double d_mod = d.module();
  bool doCollide=false;

  switch (relativePos1_)
  {
   case EXTERNAL :
     doCollide 
			= d_mod <= ( shape1_->outerRadius() + shape2_->outerRadius() );
     break;
   case THIS_CONTAINED :
     doCollide
			= d_mod >= ( shape2_->innerRadius() - shape1_->outerRadius() );
     break;
   case THIS_CONTAINER :
     doCollide
			= d_mod >= ( shape1_->innerRadius() - shape2_->outerRadius() );
     break;
   default:
     GUARANTEE(false, "RelativePosition incorrecta");
	}
  return doCollide;
}

///////////////////////////////////////////////////////////////////////////////
bool 
CollisionDetection::detectEdgePenetration( std::vector<Edge> & vEdge, 
                                           std::vector<Polygon> & vPolyg )
{
  static Coord3D intersection;
  for( unsigned int i= 0; i< vEdge.size(); ++i)
  { 
    for( unsigned int j = 0; j<vPolyg.size(); ++j)
    {
      if( intersectPolygonAndEdge( vPolyg[j], vEdge[i], intersection ) )
      {
        TRACEC(JAEPHYS_COLLID,1,"edge=" << i << " polygon=" << j);
        TRACEC(JAEPHYS_COLLID,3,"edge=\n" << vEdge[i]);
        TRACEC(JAEPHYS_COLLID,3,"polygon=\n" << vPolyg[j]);
        TRACEC(JAEPHYS_COLLID,3,"intersection=" << intersection);
        return true;
      }
    }
  }
  return false;
}
