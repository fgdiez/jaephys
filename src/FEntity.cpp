#include "FEntity.hpp"
#include "Entity.hpp"
#include "Attribute.hpp"
#include "PolygonalShape.hpp"
#include "Shape.hpp"

#include "ConstantVelocityMotion.hpp"
#include "RotationTranslationMotion.hpp"
#include "FreeRigidBodyMotion.hpp"
#include "AcceleratedMotion.hpp"

#include "FlatColorAppearence.hpp"
#include "WireAppearence.hpp"
#include "PunkWireAppearence.hpp"
//#include "TexturedAppearence.hpp"

#include "ParseTree.hpp"

#include "auxiliary.hpp"

#include <string>

using namespace jaephys;

///////////////////////////////////////////////////////////////////////////////
void FEntity::createElement( ParseTree & tree, Entity* & entity)
{
  TRACEC(JAEPHYS_ENT,1,"enter");
  vAttrib_.clear();
  mapAttrib_.clear();
  
  tree.iniIteration();
  ParseTree* child= tree.nextChild();
  entity= new Entity();

  while(child !=0)
  {
    if(child->token_ == "attrib\\")
    {
      createAttribute( *child, *entity);
    }
    else if(child->token_ == "anim")
    {
      createAnimation( *child, *entity );
    }
    else if(child->token_ == "appearence" || child->token_ == "appearence\\")
    {
      createAppearence(*child, *entity );
    }
    else
    {
      GUARANTEE(false,"token=" << child->token_ << " not contemplated");
      exit(1);
    }
    child = tree.nextChild();
  }
}

///////////////////////////////////////////////////////////////////////////////
void FEntity::createAnimation( ParseTree & tree, Entity & entity )
{
  TRACEC(JAEPHYS_ENT,2,"enter");

  if(tree.value_ == "CV_MOTION")
  {
    entity.setAnimation( new ConstantVelocityMotion( entity ) );
  }
  else if(tree.value_ == "RT_MOTION")
  {
    entity.setAnimation( new RotationTranslationMotion( entity ) );
  }
  else if(tree.value_ == "FRB_MOTION")
  {
    entity.setAnimation( new FreeRigidBodyMotion( entity ) );
  }
  else if(tree.value_ == "A_MOTION")
  {
    entity.setAnimation( new AcceleratedMotion( entity ) );
  }
  else
  {
    GUARANTEE(false,"animation="<< tree.value_ << " not contemplated");
    exit(1);
  }
}


///////////////////////////////////////////////////////////////////////////////
void FEntity::createAppearence( ParseTree& tree, Entity & entity )
{
  TRACEC(JAEPHYS_ENT,1,"enter");

  if(tree.value_ == "FLATCOLOR")
  {
    entity.setAppearence( new FlatColorAppearence( entity ) );
  }
  else if(tree.value_ == "WIRE")
  {
    entity.setAppearence( new WireAppearence( entity ) );
  }
  else if(tree.value_ == "PUNKWIRE")
  {
    entity.setAppearence( new PunkWireAppearence( entity ) );
  }
	/*
  else if( tree.value_ == "TEXTURE" )
  {
    tree.iniIteration();
    ParseTree* child= tree.nextChild();
    GUARANTEE(child != 0 && child->token_ == "file", 
							"sintax:" << endl <<
              "<appearence\\ TEXTURE>" << endl <<
              "  <file nombreFichero>"<< endl <<
              "<\appearence>");
    stringstream filename;
    filename << child->value_;
    char fich[100];
    filename >> fich;
    entity.setAppearence( new TexturedAppearence( entity ,fich ) );
  }
	*/
  else
  {
    GUARANTEE(false, "Appearence=" << tree.value_ << " not found");
    exit(1);
  }
}

///////////////////////////////////////////////////////////////////////////////
void FEntity::createAttribute( ParseTree& tree, Entity & entity )
{
  TRACEC(JAEPHYS_ENT,2,"enter");

  std::string concept;
  tree.iniIteration();
    
  for( ParseTree* child= tree.nextChild(); 
			 child !=0; 
			 child= tree.nextChild())
  {
    if( child->token_ == "concept" )
    {
      concept= child->value_;
      break;
    }
  }

	Attribute* attrib;
  if( tree.value_ == "PolygonalShape" )
  {
    createPolygonalShape( tree, concept, attrib );
  }
  else if( tree.value_ == "Coord3D" )
  {
    createCoord3d( tree, concept, attrib );
  }
  else if( tree.value_ == "double" )
  {
    createDouble( tree, concept, attrib );
  }
  else
  {
    GUARANTEE( false, "Attribute=" << tree.value_ << " not contemplated" );
  }

  if(attrib != 0)
  {
    mapAttrib_[concept.c_str()] = attrib;
		entity.addAttribute( attrib );
  }
}

///////////////////////////////////////////////////////////////////////////////
void FEntity::createPolygonalShape( ParseTree    & tree, 
																		const std::string & concept, 
																		Attribute*   & attrib)
{
  TRACEC( JAEPHYS_ENT, 2, "enter" );
  std::string shapeName;
  Coord3D position;
    
	tree.iniIteration();
  ParseTree* child = tree.nextChild();
  for( ; child != 0; child = tree.nextChild() )
  {
    if( child->token_ == "shape" )
    {
      shapeName = child->value_;
    }
    else if( child->token_ == "position" )
    {
      position= Coord3D( child->value_ );
    }
  }

  PolygonalShape* shape = new PolygonalShape();

  if(shapeName == "CUBE")
  {
    double side;
    
    for( tree.iniIteration(), child= tree.nextChild(); 
				 child!=0; 
				 child= tree.nextChild() )
    {
      if( child->token_ == "side" )
      {
				str2Double( child->value_, side );
        TRACEC( JAEPHYS_ENT,3,"side=" << side );
        break;
      }
    }
    shape->createCube( side );
  }
  else if(shapeName == "PYRAMID")
  {
    double baseDiagonal;
    double height;
    
    for( tree.iniIteration(), child= tree.nextChild()
       ; child!=0
       ; child= tree.nextChild()
       )
    {
      if( child->token_ == "baseDiagonal" )
      {
				str2Double( child->value_, baseDiagonal );
        TRACEC( JAEPHYS_ENT,3,"baseDiagonal=" << baseDiagonal );
      }
      else if( child->token_ == "height" )
      {
				str2Double( child->value_, height );
        TRACEC( JAEPHYS_ENT,3,"height=" << height );
      }
    }
    shape->createPyramid( baseDiagonal, height );
  }
  else
  {
    GUARANTEE(false,"shape=" << shapeName << " no contemplada");
    exit(1);
  }

	TAttribute<Shape>* attrShape
    = new TAttribute<Shape>( shape, concept.c_str() );

  (attrShape->attrib_)->translate( position );
  attrib = attrShape;
}

///////////////////////////////////////////////////////////////////////////////
void FEntity::createCoord3d( ParseTree    & tree, 
														 const std::string & concept, 
														 Attribute*   & attrib )
{
  TRACEC(JAEPHYS_ENT,2,"enter");
  Coord3D* c = 0;
    
  tree.iniIteration();
  ParseTree* child= tree.nextChild();
  while( child != 0 )
  {
    if( child->token_ == "value" )
    {
      c = new Coord3D( child->value_ );
    }
    child = tree.nextChild();
  }

  attrib= new TAttribute<Coord3D>( c, concept.c_str() );
}

///////////////////////////////////////////////////////////////////////////////
void FEntity::createDouble( ParseTree    & tree, 
														const std::string & concept, 
														Attribute*   & attrib )
{
  TRACEC( JAEPHYS_ENT, 2, "enter" );
  double* value = new double();
    
  tree.iniIteration();
  ParseTree* child= tree.nextChild();
  while( child != 0 )
  {
    if(child->token_ == "value")
    {
			str2Double( child->value_, *value );
    }
    child = tree.nextChild();
  }

  attrib= new TAttribute<double>( value, concept.c_str() );
}

