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

#include "FInteraction.hpp"

#include "Entity.hpp"
#include "Interaction.hpp"
#include "ParseTree.hpp"

#include "CollisionDetection.hpp"
#include "CollisionDetermination.hpp"
#include "ParticleCollision.hpp"
#include "ImpulsiveCollision.hpp"
#include "Gravitation.hpp"

#include "debugging.h"

using namespace jaephys;

void FInteraction::createElement( ParseTree      & tree, 
																	const Map_idEntity   & mapIdEntity, 
																	std::vector<Interaction*> & vInter )
{
  TRACEC(JAEPHYS_INTER,1,"enter");
  mapIdEntity_= mapIdEntity;
  
  tree.iniIteration();
  ParseTree* child= tree.nextChild();
  std::string type;  
  while(child !=0)
  {
    if(child->token_ == "type")
    {
      type = child->value_;
      break;
    }
    child = tree.nextChild();
  }

  if(type == "COLLISION")
  {
    createCollisions(tree, vInter);
  }
  else if(type == "GRAVITATION")
  {
    createGravitations(tree, vInter);
  }
}

///////////////////////////////////////////////////////////////////////////////
void FInteraction::createCollisions( ParseTree            & tree, 
																		 std::vector<Interaction*> & vInter )
{
  TRACEC(JAEPHYS_INTER,1,"enter");
  tree.iniIteration();
  std::string subtype;
  std::string container;
  std::string contained;
  std::string entities;
  RelativePosition relativePos1=EXTERNAL;
  
  ParseTree* child= tree.nextChild();
  while(child !=0)
  {
    // token subtype
    if(child->token_ == "subtype")
    {
      subtype = child->value_;
    }
    // token relation
    else if(child->token_ == "relation")
    {
      // vemos el value de la relation
      if(child->value_ == "CONTAINER-CONTAINED")
      {
        relativePos1 = THIS_CONTAINER;
      }
      else if(child->value_ == "EXTERNAL")
      {
        relativePos1 = EXTERNAL;
      }
      else
      {
        GUARANTEE(false,"relation=" << child->value_ << " unexpected");
        exit(1);
      }
    }
    // token entities
    else if(child->token_ == "entities")
    {
      entities = child->value_;
    }
    // token container
    else if(child->token_ == "container")
    {
      container= child->value_;
    }
    // token contained
    else if(child->token_ == "contained")
    {
      contained = child->value_;
    }
    // avanzamos el puntero al next child del tree(recorrido en anchura)
    child= tree.nextChild();
  }
  
  // una vez reunidos todos los values de los tokens construimos las interactiones 
  if(relativePos1 == THIS_CONTAINER)
  {
    Entity* entity= mapIdEntity_[container.c_str()];
    GUARANTEE(entity,"Entity with id="<< container<< " not found");

    std::vector<Entity* > vEntity;
    getEntities(contained, vEntity);

    createCollisionCombinations( entity, vEntity, vInter, 
																 subtype, relativePos1 );
  }
  else
  {
    std::vector<Entity* > vEntity;
    getEntities(entities, vEntity);
    createCollisionCombinations( vEntity, vInter, subtype, relativePos1);
  }
}

void FInteraction::createGravitations( ParseTree            & tree, 
																			 std::vector<Interaction*> & vInter )
{
  TRACEC(JAEPHYS_INTER,1,"enter");

  tree.iniIteration();
  std::string entities;
  
  ParseTree* child= tree.nextChild();
  while(child !=0)
  {
    // token entities
    if(child->token_ == "entities")
    {
      entities = child->value_;
    }

    child= tree.nextChild();
  }
  
  std::vector<Entity* > vEntity;
  
	getEntities(entities, vEntity);
	createGravitationCombinacions( vEntity, vInter);
}


void FInteraction::getEntities( const std::string     & ids, 
															  std::vector<Entity *> & vEntity )
{
  TRACEC(JAEPHYS_INTER,2,"enter");
  int ini=0;
  unsigned int cont=0;
  int index;

  TRACEC(JAEPHYS_INTER,2,"ids=" << ids );
  index= ids.find_first_of(", ");
  while(index != -1)
  {
    cont= index - ini;
    std::string s(ids,ini,cont); 
    Entity* entity= mapIdEntity_[s.c_str()];
    GUARANTEE(entity != 0,"entity no encontrado");
    vEntity.push_back(entity);

    ini= ids.find_first_not_of(", ",index+1);
    GUARANTEE(ini != -1, "badly formed id list");
    index= ids.find_first_of(", ",ini);
  }

  cont= ids.size() - ini;
  std::string s(ids,ini, cont);
  Entity* entity= mapIdEntity_[s.c_str()];
  GUARANTEE(entity != 0,"id="<< s << " no encontrado");
  vEntity.push_back(entity);
}

void FInteraction::createCollisionCombinations( std::vector<Entity*> & vEntity, 
																								std::vector<Interaction*> & vInter,
																								std::string & type,
																								RelativePosition relPos,
																								unsigned int index=0 )
{
  TRACEC(JAEPHYS_INTER,2,"enter");
  for( unsigned int i=index+1; i< vEntity.size(); ++i)
  {
    Interaction* inter=0;
    createCollisionInteraction( type, *(vEntity[index]), *(vEntity[i]),
																relPos,inter );
    GUARANTEE( inter, "Interaction not created");
    vInter.push_back(inter);
  }
  if( (index+2) < vEntity.size() )
  {
    createCollisionCombinations( vEntity, vInter, type, relPos, index+1 );
  }
}

///////////////////////////////////////////////////////////////////////////////
void FInteraction::createCollisionCombinations
 ( Entity* entity
 , std::vector<Entity*>& vEntity
 , std::vector<Interaction*>& vInter
 , std::string& type
 , RelativePosition relPos
 )
{
  TRACEC(JAEPHYS_INTER,2,"enter");
  for(unsigned int i=0;i<vEntity.size();++i)
  {
    Interaction* inter=0;
    createCollisionInteraction( type, *entity, *vEntity[i], relPos, inter);
    GUARANTEE(inter,"Interaction not created");
    vInter.push_back(inter);
  }
}


///////////////////////////////////////////////////////////////////////////////
// FUNCION: crear todas las combinaciones posibles de interactiones entre dos
//  entities partiendo de una lista de entities de enterda. Si la lista es de N
//  elementos, se crearán N! interactiones.
//
void FInteraction::createGravitationCombinacions
 ( std::vector<Entity*>      & vEntity
 , std::vector<Interaction*> & vInter
 , unsigned int           index
 )
{
  TRACEC(JAEPHYS_INTER,2,"enter");
  for(unsigned int i=index+1; i<vEntity.size(); ++i)
  {
    Interaction* inter=0;
    createGravitationInteraction( *(vEntity[index]), *(vEntity[i]), inter);
    GUARANTEE(inter,"Interaction not created");
    vInter.push_back(inter);
  }

  if((index+2) < vEntity.size())
  {
    createGravitationCombinacions(vEntity, vInter, index+1);
  }
}

//////////////////////////////////////////////////////////////////////////////
void FInteraction::createCollisionInteraction
 ( const std::string & type
 , Entity & entity1, Entity & entity2
 , const RelativePosition relativePos1 
 , Interaction* & interaction
 )
{
  TRACEC(JAEPHYS_INTER,1,"enter");
  CollisionDetection* detection
		= new CollisionDetection( entity1, entity2, relativePos1);

  CollisionDetermination* determination
   = new CollisionDetermination(entity1, entity2, relativePos1);

  Algorithm * resolution= 0;
  if(type == "PARTICLE")
	{
		Coord3D & normal= determination->collision_.normal;
		resolution = new ParticleCollision( entity1, entity2, normal);
	}
	/*
	else if( type == "PARTICLE-SOLID")
	{
		Collision & collision= determination->collision_;
    resolution = new ChoqueEsfericoRigido(entity1,entity2, collision);
	}
	*/
  else if(type == "IMPULSIVE")
  {
		Collision & collision= determination->collision_;
    resolution = new ImpulsiveCollision( entity1, entity2, collision);
  }
	else
	{
    GUARANTEE(0, "Not supported collision subtype="<< type);
  }

  std::vector<Algorithm* > vAlgorit;
  vAlgorit.push_back( detection );
  vAlgorit.push_back( determination );
  vAlgorit.push_back( resolution );

  interaction= new Interaction(vAlgorit);
}


//////////////////////////////////////////////////////////////////////////////
void FInteraction::createGravitationInteraction
 ( Entity & entity1, Entity & entity2
 , Interaction* & interaction
 )
{
  TRACEC(JAEPHYS_INTER,1,"enter");

  Algorithm* gravitation= new Gravitation( entity1,entity2);
  std::vector<Algorithm* > vAlgor;
  vAlgor.push_back( gravitation);

  interaction= new Interaction(vAlgor);
}
